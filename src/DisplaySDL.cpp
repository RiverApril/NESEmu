#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "DisplaySDL.hpp"
#include <time.h>


DisplaySDL::DisplaySDL(const char* title, int width, int height) {

	this->width = width;
	this->height = height;
	
	this->title = title;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
		fprintf(stderr, "SDL Init Failed: %s\n", SDL_GetError());
		errored = true;
		return;
	}

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL){
		fprintf(stderr, "SDL Window Failed: %s\n", SDL_GetError());
		errored = true;
		return;
	}
	screenSurface = SDL_GetWindowSurface(window);

	startTicks = 0;

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
	delete[] rgbGfx;
}

void DisplaySDL::drawGridAt(unsigned char (*getPoint)(int), int width, int height, unsigned char multiplier, int drawX, int drawY, int drawWidth, int drawHeight, int smoothOn, int smoothOff){
	unsigned char* rgbaGfx = new unsigned char[width*height * 4];
	for (int i = 0; i < width*height; i++){
		int j = i * 4;
		rgbaGfx[j] = getPoint(i) * multiplier;
		rgbaGfx[j + 1] = getPoint(i) * multiplier;
		rgbaGfx[j + 2] = getPoint(i) * multiplier;
		rgbaGfx[j + 3] = getPoint(i) > 0 ? smoothOn : smoothOff;
	}

	SDL_Surface* gfxSurface = SDL_CreateRGBSurfaceFrom(rgbaGfx, width, height, screenSurface->format->BitsPerPixel, width * 4, screenSurface->format->Rmask, screenSurface->format->Gmask, screenSurface->format->Bmask, 0xFF000000);
	SDL_Rect streached;
	streached.x = drawX;
	streached.y = drawY;
	streached.w = drawWidth;
	streached.h = drawHeight;
	SDL_BlitScaled(gfxSurface, NULL, screenSurface, &streached);
	delete[] rgbaGfx;
}

void DisplaySDL::drawPixelAt(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned int xScale, unsigned int yScale){
	unsigned char* rgbaGfx = new unsigned char[4];
	rgbaGfx[0] = b;
	rgbaGfx[1] = g;
	rgbaGfx[2] = r;
	rgbaGfx[3] = a;
	SDL_Surface* gfxSurface = SDL_CreateRGBSurfaceFrom(rgbaGfx, 1, 1, screenSurface->format->BitsPerPixel, 4, screenSurface->format->Rmask, screenSurface->format->Gmask, screenSurface->format->Bmask, 0xFF000000);
	SDL_Rect streached;
	streached.x = x;
	streached.y = y;
	streached.w = xScale;
	streached.h = yScale;
	SDL_BlitScaled(gfxSurface, NULL, screenSurface, &streached);
	delete[] rgbaGfx;
}


void DisplaySDL::draw(){

	SDL_UpdateWindowSurface(window);

	frames++;


}

void DisplaySDL::update(bool p1Keys[8], bool p2Keys[8]){
	SDL_SetWindowTitle(window, (title+" - FPS: "+std::to_string(displayFPS)).c_str());
	
		
	int currentTicks = SDL_GetTicks() - startTicks;
	
	if (currentTicks >= 1000){
		startTicks = SDL_GetTicks();
		displayFPS = frames;
		frames = 0;
	}
	
	
	 
	SDL_PumpEvents();

	while (SDL_PollEvent(&event)){
		bool state = false;
		switch (event.type){

			case SDL_QUIT:
				quit = true;
				break;

			case SDL_KEYDOWN:
				state = true;
				//intentionally no break
			case SDL_KEYUP:
				switch (event.key.keysym.sym){
					
					case SDLK_z:
						p1Keys[keyA] = state;
						break;
					case SDLK_x:
						p1Keys[keyB] = state;
						break;
					case SDLK_RSHIFT:
						p1Keys[keySelect] = state;
						break;
					case SDLK_RETURN:
						p1Keys[keyStart] = state;
						break;
					case SDLK_UP:
						p1Keys[keyUp] = state;
						break;
					case SDLK_DOWN:
						p1Keys[keyDown] = state;
						break;
					case SDLK_LEFT:
						p1Keys[keyLeft] = state;
						break;
					case SDLK_RIGHT:
						p1Keys[keyRight] = state;
						break;
						
					case SDLK_ESCAPE:
						quit = true;
						break;
				}
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				mouseDown = true;
				break;
			case SDL_MOUSEBUTTONUP:
				mouseDown = false;
				break;

			default:
				break;
		}
	}
	SDL_GetMouseState(&mouseX, &mouseY);
}
