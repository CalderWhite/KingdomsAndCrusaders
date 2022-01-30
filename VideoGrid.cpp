#include <iostream>

#include "VideoGrid.h"

VideoGrid::VideoGrid(int sw, int sh, double pf, int po, double pmin, double pmax)
    : Grid(sw, sh, pf, po, pmin, pmax) {

    m_palette_size = 27;
    uint8_t temp_palette[m_palette_size] = {
        // terrain
        237, 232, 175, // land (beige)
        0x00, 0x00, 0xFF, // water (pure blue)

        // colony colours
        0xFF, 0x00, 0x00, // red
        0xEE, 0x00, 0xFF, // pink
        0xFF, 0xFA, 0x00, // yellow
        0x00, 0xFF, 0x00, // green
        0xFF, 0xFF, 0xFF, // white
        0x00, 0x00, 0x00, // black
        0xFF, 0xC0, 0xCB  // light pink
    };

    m_palette = new uint8_t[m_palette_size];
    for (int i=0; i<m_palette_size; i++) {
        m_palette[i] = temp_palette[i];
    }
}

VideoGrid::~VideoGrid() {
    delete[] m_palette;
}

void VideoGrid::draw(uint8_t frame[]) {
    for (int r=0; r<m_screen_height; r++) {
        for (int c=0; c<m_screen_width; c++) {
            Person& p = m_person_grid[r][c];

            int palette_index = 0;
            if (p.getActive()) {
                palette_index = 2 + p.getColony();
            } else if (m_terrain_grid[r][c]) {
                palette_index = 0;
            } else {
                palette_index = 1;
            }

            frame[r * m_screen_width + c] = palette_index;
        }
    }
}
