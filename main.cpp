#include <iostream>

#include <SDL.h>
SDL_Window* window;

#include "Grid.h"

const int GLVersionMajor = 3;
const int GLVersionMinor = 0;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

int setup() {
	// Init
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not be initialized.\n" << SDL_GetError();
		std::cin.get();
		return 1;
	}

	// Creating SDL window
	window = SDL_CreateWindow("KingdomsAndCrusaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (window == nullptr) {

		std::cout << "Window could not be created.\n" << SDL_GetError();
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
	Grid g(10, SCREEN_WIDTH, SCREEN_HEIGHT);

	g.test();

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// int screen_height = SCREEN_HEIGHT;
	// int screen_width = SCREEN_WIDTH;
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

		// int h = 0;
		// int w = 0;
		// SDL_GetWindowSize(window, &w, &h);

		// if (h != screen_height || w != screen_width) {
		// 	g.resizeScreen(w, h);
		// 	screen_height = h;
		// 	screen_width = w;
		// }

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		g.updatePeople();
		g.draw(renderer);

		SDL_RenderPresent(renderer);
    }

	// Exit
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}