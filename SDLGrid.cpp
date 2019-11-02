#include <iostream>

#include <SDL.h>

#include "SDLGrid.h"

SDLGrid::SDLGrid(int gs, int sw, int sh, double pf, int po, double pmin, double pmax)
    : Grid(gs, sw, sh, pf, po, pmin, pmax) {
    m_rects.resize(gs, std::vector<SDL_Rect>(gs));

    resizeScreen(sw);
}

void SDLGrid::resizeScreen(int s) {
    m_screen_height = s;
    m_screen_width = s;
    rescaleGrid();
}

void SDLGrid::draw(SDL_Renderer* renderer) {
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

void SDLGrid::rescaleGrid() {
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

void SDLGrid::setDrawColorToColony(SDL_Renderer* renderer, int colony) const {
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
            SDL_SetRenderDrawColor(renderer, 255, 192, 203, 255);
            break;
        default:
            std::cerr << "Exceeded colony max!!";
            exit(1);
            break;
    }
}