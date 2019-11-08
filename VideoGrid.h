#pragma once

#include <stdint.h>

#include "Grid.h"

class VideoGrid : public Grid {
public:
    VideoGrid(int grid_size, int screen_width, int screen_height, double perlin_frequency,
         int perlin_octave, double perlin_min, double perlin_max);

    ~VideoGrid();
    VideoGrid(const VideoGrid& g) = delete;
    VideoGrid& operator= (const VideoGrid& g) = delete;
    
    void draw(uint8_t frame[]);

    uint8_t* getPalette() {
        return m_palette;
    }

private:
    // can't make use of const as it is passed to the encoder written in c
    uint8_t* m_palette;
    int m_palette_size;

    int m_width_multiple;
};
