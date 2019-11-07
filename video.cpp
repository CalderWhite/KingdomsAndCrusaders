#include <iostream>
#include <signal.h>
#include <thread>
#include <stdint.h>
#include <chrono>
#include <unistd.h>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>

#include "CVGrid.h"

const int screen_width = 1200;
const int grid_size = 10;
const int fps = 30;

bool running = true;
bool need_frame = false;
bool generating_frame = false;
int queue_index = 0;
const int queue_size = 2;

int frame_count = 0;

cv::VideoWriter video;
cv::Size video_size(screen_width, screen_width);
cv::Mat frames[queue_size];

CVGrid g(grid_size, screen_width, screen_width, 7.0, 1, 0.0, 0.45);

void signal_handler(int signum) {
    std::cerr << "Caught signal" << signum << "\n";

    running = false;
    video.release();

    exit(signum);
}

std::chrono::high_resolution_clock::time_point getTime() {
    return std::chrono::high_resolution_clock::now();
}

int getTimeDiff(
    std::chrono::high_resolution_clock::time_point t1,
    std::chrono::high_resolution_clock::time_point t2) {
    return std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
}

void gridUpdateLoop() {
    while (running) {
        // wait for encoder to finish encoding the last frame
        while (!need_frame) {
            // sleep so we don't max out the processor
            usleep(10);
        }

        ++queue_index;
        queue_index %= queue_size;

        g.updatePeople();
        g.draw(frames[queue_index]);

        need_frame = false;
    }
}

void encodeLoop() {
    auto t1 = getTime();
    while (running) {
        need_frame = true;
        video.write(frames[queue_index]);
        ++frame_count;

        if (frame_count % fps == 0) {
            auto t2 = getTime();
            float gen_speed = 1000.0/getTimeDiff(t1, t2);

            t1 = t2;

            int seconds = frame_count / fps;
            int minutes = seconds / 60;
            std::cout << "\033[K" "\033[s" <<
            "Video time: " << minutes << "m " << (seconds%60) << "s "
            "Generation Rate: " << gen_speed << "x\n"
            "\033[u";
        }

        //cv::imshow("Frame", frames[queue_index]);
        //if(cv::waitKey(30) >= 0) break;

        // wait until it is done generating the frame
        while (need_frame) {
            usleep(10);
        }
    }
}

int main(){
    g.addRandomOnLand(7, 1);

    video.open("output.webm", cv::VideoWriter::fourcc('V','P','0','9'), fps, video_size);
    //video.open("output.avi", cv::VideoWriter::fourcc('M','J','P','G'), fps, video_size);
    if (!video.isOpened()) {
        std::cerr << "Error opening video!\n";
        exit(1);
    }

    for (int i=0; i<queue_size; i++) {
        frames[i] = cv::Mat(video_size, CV_8UC3, cv::Scalar(0, 0, 0));
    }

    signal(SIGINT, signal_handler);

    g.draw(frames[0]);

    std::thread enc_loop(encodeLoop);
    std::thread grid_loop(gridUpdateLoop);

    enc_loop.join();
    grid_loop.join();
}
