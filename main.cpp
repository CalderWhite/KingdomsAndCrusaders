#include <iostream>
#include <stdio.h>
#include <stdint.h>

#include <SDL.h>
#include <SDL_ttf.h>
SDL_Window* window;

#include "Grid.h"
#include "FrameCounter.h"

// these are just the initial sizes. The grid will adjust when you resize the window
const int SCREEN_WIDTH = 2000;
const int SCREEN_HEIGHT = 2000;
const int FPS_MAX = 20;
const int TARGET_MS = 1000/FPS_MAX;

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

	// SIMULATION SPECIFIC SETUP
    bool running = true;
	int grid_size = 400;
	Grid g(grid_size, SCREEN_WIDTH, SCREEN_HEIGHT, 7.0, 1, 0.0, 0.45);
	g.addRandomOnLand(7, 1);

	// DISPLAY SETUP
	FrameCounter frame_counter;

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	TTF_Font* free_mono_font = TTF_OpenFont("fonts/freefont-20051206/FreeMonoBold.ttf", 32);
	if (!free_mono_font) {
		std::cerr << "TTF error: " << TTF_GetError() << "\n";
		exit(1);
	}
	
	SDL_Color f_color = {255, 0, 0};
	SDL_Rect fps_rect;
	fps_rect.x = 0;
	fps_rect.y = 0;
	fps_rect.h = 0;
	fps_rect.w = 0;

	SDL_Event e;
	frame_counter.init();
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

		Uint32 t1 = SDL_GetTicks();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		g.updatePeople();
		g.draw(renderer);

		frame_counter.update();

		char fps_text[9];
		snprintf(fps_text, sizeof(fps_text), "fps: %3d", static_cast<int>(frame_counter.getFps()));

		TTF_SizeText(free_mono_font, fps_text, &fps_rect.w, &fps_rect.h);

		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(free_mono_font, fps_text, f_color);
		SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
		SDL_RenderCopy(renderer, message, NULL, &fps_rect);

		SDL_RenderPresent(renderer);

		Uint32 t2 = SDL_GetTicks();
		int delay = TARGET_MS - (t2 - t1);
		if (delay > 0) {
			SDL_Delay(delay);
		}
    }

	// Exit
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}