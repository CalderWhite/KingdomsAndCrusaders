#include <iostream>
#include <iomanip>
#include <stdio.h>

#include <SDL.h>
#include <SDL_ttf.h>
SDL_Window* window;

#include "Grid.h"
#include "FrameCounter.h"

const int GLVersionMajor = 3;
const int GLVersionMinor = 0;
const int SCREEN_WIDTH = 1500;
const int SCREEN_HEIGHT = 1500;

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
	int grid_size = 300;
	Grid g(grid_size, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	Person p = Person(1);
	g.addPerson(p, 1, grid_size/2);

	FrameCounter frame_counter;

	std::cout << std::setprecision(9);
	std::cout << std::fixed;

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	TTF_Font* free_mono_font = TTF_OpenFont("fonts/freefont-20051206/FreeMonoBold.ttf", 24);
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

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		g.updatePeople();
		g.draw(renderer);

		frame_counter.update();

		char fps_text[9];
		snprintf(fps_text, sizeof(fps_text), "fps: %3d", static_cast<int>(frame_counter.getFps()));

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