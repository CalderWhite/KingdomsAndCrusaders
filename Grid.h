#pragma once

#include <vector>
#include <random>

#include "Person.h"


class Grid {
public:

    Grid(int screen_width, int screen_height, double perlin_frequency,
         int perlin_octave, double perlin_min, double perlin_max);

    void addPerson(Person& p, int row, int col);
    void addRandomOnLand(int colony_count, int n);

    void updatePeople();

protected:
    static const int m_max_colony_count = 8;
    int m_screen_width;
    int m_screen_height;

    // should be in the interval [0.1, 64.0]
    double m_perlin_frequency;

    // should be in the interval [1, 16]
    int m_perlin_octaves;

    double m_perlin_min;
    double m_perlin_max;

    int m_total_land;

    double m_colony_powers[m_max_colony_count];

    std::vector<std::vector<bool>> m_terrain_grid;
    std::vector<std::vector<Person>> m_person_grid;
    std::vector<std::vector<Person>> m_person_grid_next;
    std::vector<int> m_colony_count;

    std::random_device m_random_device;
    std::mt19937 m_random_generator;

    inline void removePerson(int row, int col) {
        // add 1 to grid selector to select the gridNext
        m_person_grid[row][col].setActive(false);

        // idk man. Maybe remove this. Something is sus
        m_person_grid_next[row][col].setActive(false);
    }

    inline bool onEdge(int row, int col) {
        if (row == 0 || row == m_screen_height-1 || col == 0 || col == m_screen_width-1) {
            return true;
        }

        return false;
    }

    inline bool outOfBounds(int row, int col) {
        if (row < 0 || row == m_screen_height || col < 0 || col == m_screen_width) {
            return true;
        }

        return false;
    }

    void updateColonyCount();

    void updatePersonState(Person& p, int row, int col);
    bool killOneEnemyNeighbor(Person& p, int row, int col);
    void attemptReproduction(Person& p, int row, int col);
};
