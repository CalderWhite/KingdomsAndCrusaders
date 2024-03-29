#include <vector>
#include <iostream>
#include <random>

#include "third_party/PerlinNoise.hpp"
#include "Grid.h"

Grid::Grid(int sw, int sh, double pf, int po, double pmin, double pmax)
    : m_screen_width(sw), m_screen_height(sh), m_perlin_frequency(pf),
    m_perlin_octaves(po), m_perlin_min(pmin), m_perlin_max(pmax), m_random_device(),
    m_random_generator((m_random_device())) {
    
    m_terrain_grid.resize(m_screen_height, std::vector<bool>(m_screen_width));
    m_person_grid.resize(m_screen_height, std::vector<Person>(m_screen_width));
    m_person_grid_next.resize(m_screen_height, std::vector<Person>(m_screen_width));

    m_terrain_grid[m_screen_height-1][0] = true;

    // generate the island with perlin noise
    uint32_t seed = m_random_generator();
    siv::PerlinNoise perlin(seed);
    double fr = m_screen_height / m_perlin_frequency;
    double fc = m_screen_width / m_perlin_frequency;

    for (int r=0; r<m_screen_height; r++) {
        for (int c=0; c<m_screen_width; c++) {
            double p_ = perlin.octaveNoise0_1(r / fr, c / fc, m_perlin_octaves);
            if (p_ > m_perlin_min && p_ < m_perlin_max) {
                m_terrain_grid[r][c] = true;
            } else {
                m_terrain_grid[r][c] = false;
            }
        }
    }

    // calculate the total land size for the birth algorithm to use later
    for (int r=0; r<m_screen_height; r++) {
        for (int c=0; c<m_screen_width; c++) {
            if (m_terrain_grid[r][c]) {
                ++m_total_land;
            }
        }
    }

    m_colony_count.resize(m_max_colony_count, 0);
}

void Grid::addRandomOnLand(int colony_count, int n) {
    for (int i=0; i<colony_count; i++) {
        for (int j=0; j<n; j++) {
            std::uniform_int_distribution<int> distribution(0, m_screen_height*m_screen_width-1);
            int rand = distribution(m_random_generator);
            int r = rand / m_screen_width;
            int c = rand % m_screen_height;

            while (!m_terrain_grid[r][c]) {
                std::uniform_int_distribution<int> distribution(0, m_screen_height*m_screen_width-1);
                int rand = distribution(m_random_generator);
                r = rand / m_screen_width;
                c =  rand % m_screen_height;
            }

            Person p(i);
            addPerson(p, r, c);
        }
    }
}

void Grid::addPerson(Person& p, int row, int col) {
    m_person_grid_next[row][col] = p;
}

void Grid::updatePeople() {
    // copy over the current state to the next state before processing
    for (int r=0; r<m_screen_height; r++) {
        for (int c=0; c<m_screen_width; c++) {
            m_person_grid[r][c] = m_person_grid_next[r][c];
        }
    }

    updateColonyCount();

    for (int r=0; r<m_screen_height; r++) {
        for (int c=0; c<m_screen_width; c++) {
            Person& p = m_person_grid[r][c];

            if (p.getActive()) {
                bool murderer = killOneEnemyNeighbor(p, r, c);
                if (!murderer) {
                    updatePersonState(p, r, c);
                    switch(p.getType()) {
                        case PersonType::Breeder:
                            attemptReproduction(p, r, c);
                            break;
                        case PersonType::Sailor: {
                            char r_dir;
                            char c_dir;
                            p.getDirection(&r_dir, &c_dir);


                // remove where the sailor was before checking if it can go forward
                // this way sailors can sail off the map.
                            m_person_grid_next[r][c].setActive(false);

                            bool on_edge = onEdge(r, c);
                            if (on_edge) {
                                if (outOfBounds(r + r_dir, c + c_dir)) {
                                    continue;
                                }
                            }

                            m_person_grid_next[r+r_dir][c+c_dir] = p;
                            break;
                        } case PersonType::Settler:
                            attemptReproduction(p, r, c);
                            break;
                    }
                }
            }
        }
    }
}

void Grid::updateColonyCount() {
    for (int i=0; i<static_cast<int>(m_colony_count.size()); i++) {
        m_colony_count[i] = 0;
    }

    for (int r=0; r<m_screen_height; r++) {
        for (int c=0; c<m_screen_width; c++) {
            Person& p = m_person_grid[r][c];
            if (p.getActive()) {
                ++m_colony_count[p.getColony()];
            }
        }
    }

    for (int i=0; i<m_max_colony_count; i++) {
        double ownership = static_cast<double>(m_colony_count[i])/m_total_land;
        m_colony_powers[i] = 0.1 + 0.2*ownership;
    }
}

void Grid::updatePersonState(Person& p, int row, int col) {
    switch (p.getType()) {
        case PersonType::Breeder: {
            int directions[8][2] = {0};
            int direction_count = 0;
            bool on_edge = onEdge(row, col);
            for (int i=-1; i<=1; i++) {
                for (int j=-1; j<=1; j++) {
                    if (i | j) {
                        if (on_edge) {
                            if (outOfBounds(row + i, col + j)) {
                                continue;
                            }
                        }

                        if (!m_terrain_grid[row + i][col + j]) {
                            directions[direction_count][0] = i;
                            directions[direction_count][1] = j;

                            ++direction_count;
                        }
                    }
                }
            }

            if (direction_count > 0) {
                std::uniform_int_distribution<int> distribution(0, direction_count-1);
                int* direction = directions[distribution(m_random_generator)];
                int r = direction[0];
                int c = direction[1];

                p.setSails(r, c);
            }
            break;
        }
        case PersonType::Sailor:
            if (m_terrain_grid[row][col]) {
                p.settle();
            }
            break;
        case PersonType::Settler:
            break;
    }
}

bool Grid::killOneEnemyNeighbor(Person& p, int row, int col) {

    int directions[8][2] = {0};
    int direction_count = 0;

    bool on_edge = onEdge(row, col);
    for (int i=-1; i<=1; i++) {
        for (int j=-1; j<=1; j++) {
            if (i | j) {
                if (on_edge) {
                    if (outOfBounds(row + i, col + j)) {
                        continue;
                    }
                }

                Person& enemy = m_person_grid[row + i][col + j];
                if (enemy.getActive() && enemy.getColony() != p.getColony()) {
                    directions[direction_count][0] = i;
                    directions[direction_count][1] = j;

                    ++direction_count;
                }
            }
        }
    }

    if (direction_count > 0) {
        std::uniform_int_distribution<int> distribution(0, direction_count-1);
        int* direction = directions[distribution(m_random_generator)];
        int r = direction[0];
        int c = direction[1];

        removePerson(row, col);
        removePerson(row + r, col + c);

        return true;
    } else {
        return false;
    }
}

void Grid::attemptReproduction(Person& p, int row, int col) {
    double birth_probability = m_colony_powers[p.getColony()];

    std::uniform_real_distribution<double> distribution(0,1);
    double choice = distribution(m_random_generator);

    if (choice <= birth_probability) {
        int directions[8][2] = {0};
        int direction_count = 0;

        bool on_edge = onEdge(row, col);
        for (int i=-1; i<=1; i++) {
            for (int j=-1; j<=1; j++) {
                if (i | j) {
                    if (on_edge) {
                        if (outOfBounds(row + i, col + j)) {
                            continue;
                        }
                    }

                    if (m_terrain_grid[row + i][col + j]) {
                        Person& other1 = m_person_grid[row + i][col + j];
                        Person& other2 = m_person_grid_next[row + i][col + j];
                        if (!other1.getActive() && !other2.getActive()) {
                            directions[direction_count][0] = i;
                            directions[direction_count][1] = j;

                            ++direction_count;
                        }
                    }
                }
            }
        }

        if (direction_count > 0) {
            std::uniform_int_distribution<int> distribution(0, direction_count-1);
            int* direction = directions[distribution(m_random_generator)];
            int r = direction[0];
            int c = direction[1];

            Person& child = m_person_grid_next[row + r][col + c];
            child.setActive(true);
            child.setColony(p.getColony());
        }
    }
}
