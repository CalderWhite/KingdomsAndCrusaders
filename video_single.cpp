#include <chrono>
#include <iostream>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

extern "C" {
    #include "third_party/gifenc/gifenc.h"
}

#include "VideoGrid.h"

const int screen_width = 400;
const int grid_size = 400;
const int ms_delay = 10;
const float fps = 100/ms_delay;

bool running = true;
bool need_frame = false;

int frame_count = 0;

VideoGrid g(grid_size, screen_width, screen_width, 6.0, 1, 0.0, 0.45);
ge_GIF* gif;

std::chrono::high_resolution_clock::time_point getTime() {
    return std::chrono::high_resolution_clock::now();
}

int getTimeDiff(
    std::chrono::high_resolution_clock::time_point t1,
    std::chrono::high_resolution_clock::time_point t2) {
    return std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
}

void signal_handler(int signum) {
    std::cerr << "\n\nCaught signal " << signum << ", safely exiting...\n";

    running = false;

    ge_close_gif(gif);

    exit(signum);
}

int main(int argc, char* argv[]){
    if (argc < 2) {
        std::cerr << "No file name supplied!\n";
        exit(1);
    }
    g.addRandomOnLand(7, 1);

    gif = ge_new_gif(argv[1], screen_width, screen_width, g.getPalette(), 4, 0);

    signal(SIGINT, signal_handler);


    g.draw(gif->frame);
    ge_add_frame(gif, ms_delay);

    auto t1 = getTime();
    while (running) {
        g.updatePeople();
        g.draw(gif->frame);

        ge_add_frame(gif, ms_delay);

        ++frame_count;

        if (frame_count % static_cast<int>(fps) == 0) {
            // this calculation works since this is called only once every second of video is generated.
            // so, the difference in time between the last time it is called and this time is 1 second if we are
            // running at 1x speed.
            auto t2 = getTime();
            float gen_speed = 1000.0/getTimeDiff(t1, t2);

            t1 = t2;

            int seconds = static_cast<float>(frame_count) / fps;
            int minutes = seconds / 60;
            printf("\033[K" "\033[s");
            std::cout << "Video time: " << minutes << "m " << (seconds%60) << "s "
            "Generation Rate: " << gen_speed << "x\n";
            printf("\033[u");
        }
    }
}
