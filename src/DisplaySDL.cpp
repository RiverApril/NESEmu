#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "DisplaySDL.hpp"
#include <time.h>


DisplaySDL::DisplaySDL(const char* title, int width, int height) {

	this->width = width;
	this->height = height;

	for (int i = 0; i < 16; i++){
		keys[i] = false;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
		printf("SDL Init Failed: %s\n", SDL_GetError());
		errored = true;
		return;
	}

	window = SDL_CreateWindow("Chip-8 Emu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL){
		printf("SDL Window Failed: %s\n", SDL_GetError());
		errored = true;
		return;
	}
	screenSurface = SDL_GetWindowSurface(window);

	startTicks = clock();

}

DisplaySDL::~DisplaySDL(){
	SDL_FreeSurface(gfxSurface);
	gfxSurface = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}

void DisplaySDL::drawGridAt(const unsigned char* grid, int width, int height, unsigned char multiplier, int drawX, int drawY, int drawWidth, int drawHeight, int smoothOn, int smoothOff){
	unsigned char* rgbGfx = new unsigned char[width*height * 4];
	for (int i = 0; i < width*height; i++){
		int j = i * 4;
		rgbGfx[j] = grid[i] * multiplier;
		rgbGfx[j + 1] = grid[i] * multiplier;
		rgbGfx[j + 2] = grid[i] * multiplier;
		rgbGfx[j + 3] = grid[i] > 0 ? smoothOn : smoothOff;
	}

	SDL_Surface* gfxSurface = SDL_CreateRGBSurfaceFrom(rgbGfx, width, height, screenSurface->format->BitsPerPixel, width * 4, screenSurface->format->Rmask, screenSurface->format->Gmask, screenSurface->format->Bmask, 0xFF000000);
	SDL_Rect streached;
	streached.x = drawX;
	streached.y = drawY;
	streached.w = drawWidth;
	streached.h = drawHeight;
	SDL_BlitScaled(gfxSurface, NULL, screenSurface, &streached);
}


void DisplaySDL::draw(){

	SDL_UpdateWindowSurface(window);

	frames++;

	int currentTicks = clock() - startTicks;

	if (currentTicks < TICKS_PER_FRAME){
		SDL_Delay(TICKS_PER_FRAME - currentTicks);
	}
	startTicks = clock();
	if (frames > FPS_LIMIT){
		displayFPS = frames;
		frames = 0;
	}

}

void DisplaySDL::update(){
	SDL_PumpEvents();

	while (SDL_PollEvent(&event)){
		bool state = false;
		switch (event.type){

			case SDL_QUIT:
				quit = true;

			case SDL_KEYDOWN:
				state = true;
			case SDL_KEYUP:
				switch (event.key.keysym.sym){
					case SDLK_0:
						keys[0x0] = state;
						break;
					case SDLK_1:
						keys[0x1] = state;
						break;
					case SDLK_2:
						keys[0x2] = state;
						break;
					case SDLK_3:
						keys[0x3] = state;
						break;
					case SDLK_4:
						keys[0x4] = state;
						break;
					case SDLK_5:
						keys[0x5] = state;
						break;
					case SDLK_6:
						keys[0x6] = state;
						break;
					case SDLK_7:
						keys[0x7] = state;
						break;
					case SDLK_ESCAPE:
						quit = true;
						break;
				}
				break;

			default:
				break;
		}
	}
}
