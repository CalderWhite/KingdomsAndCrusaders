#include <chrono>
#include <iostream>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <thread>
#include <unistd.h>

extern "C" {
    #include "third_party/gifenc/gifenc.h"
}

#include "VideoGrid.h"

const int screen_width = 800;
const int grid_size = 200;
const int ms_delay = 3;
const int fps = 100/ms_delay;

bool running = true;
bool need_frame = false;

int frame_count = 0;

VideoGrid g(grid_size, screen_width, screen_width, 7.0, 1, 0.0, 0.45);
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
    std::cerr << "Caught signal" << signum << "\n";

    ge_close_gif(gif);

    exit(signum);
}

void gridUpdateLoop() {
    while (running) {
        // wait for encoder to finish encoding the last frame
        while (!need_frame) {
            // sleep so we don't max out the processor
            usleep(10);
        }

        g.updatePeople();
        g.draw(gif->frame);

        need_frame = false;
    }
}

void encodeLoop() {
    auto t1 = getTime();
    while (running) {
        need_frame = true;

        ge_add_frame(gif, ms_delay);

        ++frame_count;

        if (frame_count % fps == 0) {
            auto t2 = getTime();
            float gen_speed = 1000.0/getTimeDiff(t1, t2);

            t1 = t2;

            int seconds = frame_count / fps;
            int minutes = seconds / 60;
            printf("\033[K" "\033[s");
            std::cout << "Video time: " << minutes << "m " << (seconds%60) << "s "
            "Generation Rate: " << gen_speed << "x\n";
            printf("\033[u");
        }

        // wait until it is done generating the frame
        while (need_frame) {
            usleep(10);
        }
    }
}

int main(){
    g.addRandomOnLand(7, 1);

    gif = ge_new_gif("output.gif", screen_width, screen_width, g.getPalette(), 4, 0);

    signal(SIGINT, signal_handler);

    std::thread enc_loop(encodeLoop);
    std::thread grid_loop(gridUpdateLoop);

    char c;
    while (c != 'q') {
        std::cin >> c;
    }

    running = false;
    enc_loop.join();
    grid_loop.join();

    signal_handler(0);
}
