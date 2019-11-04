#include <iostream>

#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>

#include "CVGrid.h"

CVGrid::CVGrid(int gs, int sw, int sh, double pf, int po, double pmin, double pmax)
    : Grid(gs, sw, sh, pf, po, pmin, pmax) {
    m_rects.resize(gs, std::vector<cv::Rect>(gs));

    int l = m_screen_width / m_grid_size;
    for (int r=0; r<m_grid_size; r++) {
        for (int c=0; c<m_grid_size; c++) {
            cv::Rect rect(l*c, l*r, l, l);
            m_rects[r][c] = rect;
        }
    }

    m_land_color = cv::Scalar(175, 232, 237);
    m_water_color = cv::Scalar(255, 0, 0);

    m_colony_colors.emplace_back(0, 0, 255);
    m_colony_colors.emplace_back(255, 0, 238);
    m_colony_colors.emplace_back(0, 250, 255);
    m_colony_colors.emplace_back(0, 255, 0);
    m_colony_colors.emplace_back(255, 255, 255);
    m_colony_colors.emplace_back(0, 0, 0);
    m_colony_colors.emplace_back(203, 192, 255);
}

void CVGrid::draw(cv::Mat& frame) {
    for (int r=0; r<m_grid_size; r++) {
        for (int c=0; c<m_grid_size; c++) {
            Person& p = m_person_grid[r][c];
            if (p.getActive()) {
                cv::rectangle(frame, m_rects[r][c], m_colony_colors[p.getColony()], -1);
            } else if (m_terrain_grid[r][c]) {
                cv::rectangle(frame, m_rects[r][c], m_land_color, -1);
            } else {
                cv::rectangle(frame, m_rects[r][c], m_water_color, -1);
            }
        }
    }
}