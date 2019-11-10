#include <vector>
#include <iostream>
#include <random>

#include "third_party/PerlinNoise.hpp"
#include "Grid.h"

Grid::Grid(int gs, int sw, int sh, double pf, int po, double pmin, double pmax)
    : m_grid_size(gs), m_screen_width(sw), m_screen_height(sh), m_perlin_frequency(pf),
    m_perlin_octaves(po), m_perlin_min(pmin), m_perlin_max(pmax), m_random_device(),
    m_random_generator((m_random_device())) {
    
    m_terrain_grid.resize(m_grid_size, std::vector<bool>(m_grid_size));
    m_person_grid.resize(m_grid_size, std::vector<Person>(m_grid_size));
    m_person_grid_next.resize(m_grid_size, std::vector<Person>(m_grid_size));

    m_terrain_grid[m_grid_size-1][0] = true;

    // generate the island with perlin noise
	uint32_t seed = m_random_generator();
	siv::PerlinNoise perlin(seed);
	double fr = m_grid_size / m_perlin_frequency;
	double fc = m_grid_size / m_perlin_frequency;

    for (int r=0; r<m_grid_size; r++) {
        for (int c=0; c<m_grid_size; c++) {
            double p_ = perlin.octaveNoise0_1(r / fr, c / fc, m_perlin_octaves);
            if (p_ > m_perlin_min && p_ < m_perlin_max) {
                m_terrain_grid[r][c] = true;
            } else {
                m_terrain_grid[r][c] = false;
            }
        }
    }

    // calculate the total land size for the birth algorithm to use later
    for (int r=0; r<m_grid_size; r++) {
        for (int c=0; c<m_grid_size; c++) {
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
            std::uniform_int_distribution<int> distribution(0, m_grid_size*m_grid_size-1);
            int rand = distribution(m_random_generator);
            int r = rand / m_grid_size;
            int c =  rand % m_grid_size;

            while (!m_terrain_grid[r][c]) {
                std::uniform_int_distribution<int> distribution(0, m_grid_size*m_grid_size-1);
                int rand = distribution(m_random_generator);
                r = rand / m_grid_size;
                c =  rand % m_grid_size;
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
    for (int r=0; r<m_grid_size; r++) {
        for (int c=0; c<m_grid_size; c++) {
            m_person_grid[r][c] = m_person_grid_next[r][c];
        }
    }

    updateColonyCount();

    for (int r=0; r<m_grid_size; r++) {
        for (int c=0; c<m_grid_size; c++) {
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

                            bool on_edge = onEdge(r, c);
                            if (on_edge) {
                                if (outOfBounds(r + r_dir, c + c_dir)) {
                                    continue;
                                }
                            }

                            m_person_grid_next[r+r_dir][c+c_dir] = p;

                            m_person_grid_next[r][c].setActive(false);
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

    for (int r=0; r<m_grid_size; r++) {
        for (int c=0; c<m_grid_size; c++) {
            Person& p = m_person_grid[r][c];
            if (p.getActive()) {
                ++m_colony_count[p.getColony()];
            }
        }
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
                    if (outOfBounds(row + i, col + j) {
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
    double colony_power = m_colony_count[p.getColony()];

    double birth_probability = 0.1 + 0.3*(colony_power/m_total_land);
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
