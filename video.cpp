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

bool running = true;

int frame_count = 0;

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

int main(int argc, char* argv[]) {
    if (argc < 7) {
        std::cerr << "./video [filename] [width] [height] [ms delay between gif frames] [perlin_frequency] [perlin octave] [max frame, optional]\n";
        exit(1);
    }
    int screen_width = atoi(argv[2]);
    int screen_height = atoi(argv[3]);
    int ms_delay = atoi(argv[4]);
    float fps = 100.0 / ms_delay;
    float perlin_frequency = atof(argv[5]);
    float perlin_octave = atof(argv[6]);
    int max_frame = -1;
    if (argc > 7) {
        max_frame = atoi(argv[7]);
    }
    // further configuration of the simulation can be adjusted via the floats below.
    VideoGrid g(screen_width, screen_height, perlin_frequency, perlin_octave, 0.0, 0.45);

    g.addRandomOnLand(7, 1);

    gif = ge_new_gif(argv[1], screen_width, screen_height, g.getPalette(), 4, 0);

    signal(SIGINT, signal_handler);

    // draw the map first
    g.draw(gif->frame);
    ge_add_frame(gif, ms_delay);

    auto t1 = getTime();
    while (running && (max_frame == -1 || frame_count < max_frame)) {
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

    ge_close_gif(gif);
}
