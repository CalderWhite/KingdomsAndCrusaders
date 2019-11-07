#include <iostream>
#include <signal.h>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>

#include "CVGrid.h"

cv::VideoWriter video;

void signal_handler(int signum) {
    std::cerr << "Caught signal" << signum << "\n";

    video.release();

    exit(signum);
}

int main(){
    int screen_width = 1200;
    int grid_size = 400;
    int fps = 30;

    CVGrid g(grid_size, screen_width, screen_width, 7.0, 1, 0.0, 0.45);
    g.addRandomOnLand(7, 1);

    cv::Size video_size(screen_width, screen_width);
    video.open("output.webm", cv::VideoWriter::fourcc('V','P','0','9'), fps, video_size);

    signal(SIGINT, signal_handler);

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
