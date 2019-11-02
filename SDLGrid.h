#pragma once

#include <vector>

#include <SDL.h>

#include "Grid.h"

class SDLGrid : public Grid {
public:
    SDLGrid(int grid_size, int screen_width, int screen_height, double perlin_frequency,
         int perlin_octave, double perlin_min, double perlin_max);
    
    void resizeScreen(int s);
    void draw(SDL_Renderer* renderer);

private:
    std::vector<std::vector<SDL_Rect>> m_rects;

    void rescaleGrid();
    void setDrawColorToColony(SDL_Renderer* renderer, int colony) const;
};