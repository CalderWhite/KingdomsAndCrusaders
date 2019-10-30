#include <vector>
#include <iostream>
#include <random>

#include "Grid.h"
#include "Position.h"

Grid::Grid(int gs, int sw, int sh)
    : m_grid_size(gs), m_screen_width(sw), m_screen_height(sh), m_random_device(),
    m_random_generator((m_random_device())) {
    
    m_terrain_grid.resize(m_grid_size, std::vector<bool>(m_grid_size));
    m_person_grid.resize(m_grid_size, std::vector<Person>(m_grid_size));
    m_person_grid_next.resize(m_grid_size, std::vector<Person>(m_grid_size));
    m_rects.resize(m_grid_size, std::vector<SDL_Rect>(m_grid_size));

    m_terrain_grid[m_grid_size-1][0] = true;

    for (int r=0; r<m_grid_size; r++) {
        for (int c=0; c<m_grid_size; c++) {
            // if (r == c) {
                m_terrain_grid[r][c] = true;
            // }
        }
    }

    // calculate the total land size for the birth algorithm to use later
    for (int r=0; r<m_grid_size; r++) {
        for (int c=0; c<m_grid_size; c++) {
            if (m_terrain_grid[r][c]) {
                ++total_land;
            }
        }
    }

    // init all the rects
    rescaleGrid();
}

void Grid::resizeScreen(int s) {
    m_screen_height = s;
    m_screen_width = s;
    rescaleGrid();
}

void Grid::updatePeople() {
    // copy over the current state to the next state before processing
    for (int r=0; r<m_grid_size; r++) {
        for (int c=0; c<m_grid_size; c++) {
            m_person_grid[r][c] = m_person_grid_next[r][c];
        }
    }

    for (int r=0; r<m_grid_size; r++) {
        for (int c=0; c<m_grid_size; c++) {
            Person& p = m_person_grid[r][c];

            if (p.getActive()) {
                bool murderer = killOneEnemyNeighbor(r, c, p);

                if (!murderer) {
                    // TODO
                    switch(p.getType()) {
                        case PersonType::Breeder:
                            break;
                        case PersonType::Sailor:
                            break;
                        case PersonType::Settler:
                            break;
                    }
                }
            }
        }
    }
}

void Grid::draw(SDL_Renderer* renderer) {
    for (int r=0; r<m_grid_size; r++) {
        for (int c=0; c<m_grid_size; c++) {
            Person& p = m_person_grid[r][c];
            if (p.getActive()) {
                setDrawColorToColony(renderer, p.getColony());
                // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            } else if (m_terrain_grid[r][c]) {
                SDL_SetRenderDrawColor(renderer, 237, 232, 175, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            }

            SDL_RenderFillRect(renderer, &m_rects[r][c]);
        }
    }
}

void Grid::test() {
    Person p = Person(0);
    Person p2 = Person(1);
    Person p3 = Person(1);

    m_person_grid[3][3] = p;
    m_person_grid_next[3][3] = p;

    m_person_grid[3][4] = p2;
    m_person_grid_next[3][4] = p2;
    m_person_grid[2][2] = p3;
    m_person_grid_next[2][2] = p3;
}

void Grid::rescaleGrid() {
    int l = m_screen_width / m_grid_size;
    for (int r=0; r<m_grid_size; r++) {
        for (int c=0; c<m_grid_size; c++) {
            SDL_Rect& rect = m_rects[r][c];
            rect.w = l;
            rect.h = l;
            rect.x = l*c;
            rect.y = l*r;
        }
    }
}

void Grid::setDrawColorToColony(SDL_Renderer* renderer, int colony) const {
    switch (colony) {
        case 0:
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            break;
        case 1:
            SDL_SetRenderDrawColor(renderer, 238, 0, 255, 255);
            break;
        case 2:
            SDL_SetRenderDrawColor(renderer, 255, 250, 0, 255);
            break;
        case 3:
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            break;
        case 4:
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            break;
        case 5:
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            break;
        case 6:
            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
            break;
        case 7:
            SDL_SetRenderDrawColor(renderer, 255, 192, 203, 255);
            break;
    }
}

bool Grid::killOneEnemyNeighbor(int row, int col, Person& p) {

    int directions[9][2] = {0};
    int direction_count = 0;
    
    for (int i=-1; i<=1; i++) {
        for (int j=-1; j<=1; j++) {
            if (i | j) {
                if (row + i > -1 && row + i < m_grid_size && col + j > -1 && col + j < m_grid_size) {
                    Person& enemy = m_person_grid[row + i][col + j];
                    if (enemy.getActive() && enemy.getColony() != p.getColony()) {
                        directions[direction_count][0] = i;
                        directions[direction_count][1] = j;

                        ++direction_count;
                    }
                }
            }
        }
    }

    // std::cout << direction_count << "\n";

    if (direction_count > 0) {
        std::uniform_int_distribution<int> distribution(0,direction_count);
        int* direction = directions[distribution(m_random_generator)];
        int r = direction[0];
        int c = direction[1];

        std::cout << "Removing a mf. My Colony is: " << p.getColony() << "\n";
        std::cout << r << " " << c << "\n";

        removePerson(row, col);
        removePerson(row + r, col + c);

        return true;
    } else {
        return false;
    }
}