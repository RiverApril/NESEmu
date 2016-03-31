
#ifndef __NESEmu__DisplaySDL__
#define __NESEmu__DisplaySDL__

#include "SDL2/SDL.h"
#include <string>

#define keyA 0
#define keyB 1
#define keySelect 2
#define keyStart 3
#define keyUp 4
#define keyDown 5
#define keyLeft 6
#define keyRight 7

#define EXIT_SDL_QUIT 100

struct DisplaySDL {

	int width, height;
	std::string title;
	
	int mouseX, mouseY;
	int mouseDown = false;

	bool errored = false;
	bool quit = false;

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Surface* screenSurface = NULL;
	SDL_Surface* gfxSurface = NULL;
	SDL_Surface* gfxSurfaceStreached = NULL;

	SDL_Event event;

	const int FPS_LIMIT = 60;
	const int TICKS_PER_FRAME = 1000 / FPS_LIMIT;
	int frames = 0;
	int startTicks;
	int displayFPS = 0;

	int scale = 10;

	DisplaySDL(const char* title, int width, int height);
	virtual ~DisplaySDL();

	virtual void drawGridAt(const unsigned char* grid, int width, int height, unsigned char multiplier, int drawX, int drawY, int drawWidth, int drawHeight, int smoothOn = 0xFF, int smoothOff = 0xFF);
	virtual void drawGridAt(unsigned char (*getPoint)(int), int width, int height, unsigned char multiplier, int drawX, int drawY, int drawWidth, int drawHeight, int smoothOn = 0xFF, int smoothOff = 0xFF);
	virtual void drawPixelAt(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned int xScale = 1, unsigned int yScale = 1);
	virtual void draw();
	virtual void update(bool p1Keys[8], bool p2Keys[8]);

};

#endif /* defined(__NESEmu__DisplaySDL__) */
