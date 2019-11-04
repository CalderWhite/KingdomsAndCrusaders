#include <iostream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>

#include "CVGrid.h"

// due to the complexities of video encoding, ffmpeg is required to run this

int main(){
    int screen_width = 400;
    int grid_size = 400;
    int fps = 30;

    CVGrid g(grid_size, screen_width, screen_width, 7.0, 1, 0.0, 0.45);
    g.addRandomOnLand(7, 1);

    cv::Size video_size(screen_width, screen_width);
    cv::VideoWriter video("output.avi", cv::VideoWriter::fourcc('F', 'M', 'P', '4'), fps, video_size);
    
    cv::Mat frame(video_size, CV_8UC3, cv::Scalar(0, 0, 0));
    for (int frame_index=0;;frame_index++) {
        g.updatePeople();
        g.draw(frame);

        video.write(frame);

        if (frame_index % 120 == 0) {
            int seconds = frame_index/fps;
            std::cout << seconds/60 << "m " << (seconds%60) << "s\n";
        }
    }

    video.release();
    return 0;
}