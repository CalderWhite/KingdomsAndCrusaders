#pragma once

#include <vector>
#include <random>

#include <SDL.h>

#include "Person.h"


class Grid {
public:

    Grid(int grid_size, int screen_width, int screen_height);

    void resizeScreen(int s);

    void updatePeople();
    void draw(SDL_Renderer* renderer);
    void test();

private:
    const int m_grid_size;
    int m_screen_width;
    int m_screen_height;

    int total_land;

    std::vector<std::vector<bool>> m_terrain_grid;
    std::vector<std::vector<Person>> m_person_grid;
    std::vector<std::vector<Person>> m_person_grid_next;

    std::vector<std::vector<SDL_Rect>> m_rects;

    std::random_device m_random_device;
    std::mt19937 m_random_generator;

    inline void removePerson(int row, int col) {
        // add 1 to grid selector to select the gridNext
        m_person_grid[row][col].setActive(false);

        // idk man. Maybe remove this. Something is sus
        m_person_grid_next[row][col].setActive(false);
    }

    void rescaleGrid();
    void setDrawColorToColony(SDL_Renderer* renderer, int colony) const;

    bool killOneEnemyNeighbor(int row, int col, Person& p);
};