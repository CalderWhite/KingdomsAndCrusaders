#include <iostream>
#include <iomanip>
#include <stdio.h>

#include <SDL.h>
#include <SDL_ttf.h>
SDL_Window* window;

#include "Grid.h"

const int GLVersionMajor = 3;
const int GLVersionMinor = 0;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

// How many frames time values to keep
// The higher the value the smoother the result is...
// Don't make it 0 or less :)
#define FRAME_VALUES 10

// An array to store frame times:
Uint32 frametimes[FRAME_VALUES];

// Last calculated SDL_GetTicks
Uint32 frametimelast;

// total frames rendered
Uint32 framecount;

// the value you want
float framespersecond;

// This function gets called once on startup.
void fpsinit() {

        // Set all frame times to 0ms.
        memset(frametimes, 0, sizeof(frametimes));
        framecount = 0;
        framespersecond = 0;
        frametimelast = SDL_GetTicks();

}

void fpsthink() {

        Uint32 frametimesindex;
        Uint32 getticks;
        Uint32 count;
        Uint32 i;

        // frametimesindex is the position in the array. It ranges from 0 to FRAME_VALUES.
        // This value rotates back to 0 after it hits FRAME_VALUES.
        frametimesindex = framecount % FRAME_VALUES;

        // store the current time
        getticks = SDL_GetTicks();

        // save the frame time value
        frametimes[frametimesindex] = getticks - frametimelast;

        // save the last frame time for the next fpsthink
        frametimelast = getticks;

        // increment the frame count
        framecount++;

        // Work out the current framerate

        // The code below could be moved into another function if you don't need the value every frame.

        // I've included a test to see if the whole array has been written to or not. This will stop
        // strange values on the first few (FRAME_VALUES) frames.
        if (framecount < FRAME_VALUES) {

                count = framecount;

        } else {

                count = FRAME_VALUES;

        }

        // add up all the values and divide to get the average frame time.
        framespersecond = 0;
        for (i = 0; i < count; i++) {

                framespersecond += frametimes[i];

        }

        framespersecond /= count;

        // now to make it an actual frames per second value...
        framespersecond = 1000.f / framespersecond;

}

int setup() {
	// Init
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not be initialized.\n" << SDL_GetError();
		std::cin.get();
		return 1;
	}

	if (TTF_Init() < 0) {
		std::cerr << "SDL_ttf could not be initialized.\n" << TTF_GetError();
		return 1;
	}

	// Creating SDL window
	window = SDL_CreateWindow("KingdomsAndCrusaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (window == nullptr) {

		std::cerr << "Window could not be created.\n" << SDL_GetError();
		std::cin.get();
		return -1;

	}
	std::cout << "SDL window created successfully.\n";

	return 0;
}

int main(int argc, char* argv[]) {

	int code = setup();
	if (code != 0) {
		return code;
	}

    SDL_Event e;
    bool running = true;
	Grid g(300, SCREEN_WIDTH, SCREEN_HEIGHT);

	g.test();
	std::cout << std::setprecision(9);
	std::cout << std::fixed;

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


	TTF_Font* free_mono_font = TTF_OpenFont("fonts/freefont-20051206/FreeSans.ttf", 24);
	if (!free_mono_font) {
		std::cerr << "TTF error: " << TTF_GetError() << "\n";
		exit(1);
	}
	
	SDL_Color c_black = {0, 0, 0};
	SDL_Rect fps_rect;
	fps_rect.x = 0;
	fps_rect.y = 0;
	fps_rect.h = 0;
	fps_rect.w = 0;

	fpsinit();
    while (running) {
        while (SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYUP:
                    switch(e.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                    }
                    break;
				case SDL_WINDOWEVENT:
					switch(e.window.event) {
						case SDL_WINDOWEVENT_RESIZED:
							int s = e.window.data1 < e.window.data2 ? e.window.data1 : e.window.data2;
							g.resizeScreen(s);
						break;
					}
					break;
            }
        }

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		g.updatePeople();
		g.draw(renderer);

		fpsthink();

		char fps_text[9];
		snprintf(fps_text, sizeof(fps_text), "fps: %3d", static_cast<int>(framespersecond));

		TTF_SizeText(free_mono_font, fps_text, &fps_rect.w, &fps_rect.h);

		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(free_mono_font, fps_text, c_black);
		SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
		SDL_RenderCopy(renderer, message, NULL, &fps_rect);

		SDL_RenderPresent(renderer);

		// SDL_Delay(100);
    }

	// Exit
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}