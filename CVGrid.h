#pragma once

#include <vector>

#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>

#include "Grid.h"

class CVGrid : public Grid {
public:
    CVGrid(int grid_size, int screen_width, int screen_height, double perlin_frequency,
         int perlin_octave, double perlin_min, double perlin_max);

    void draw(cv::Mat& frame);

private:
    std::vector<std::vector<cv::Rect>> m_rects;
    std::vector<cv::Scalar> m_colony_colors;

    cv::Scalar m_land_color;
    cv::Scalar m_water_color;
};