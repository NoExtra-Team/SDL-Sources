/*
This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.

Also modified for VSC by Zorro 2^NoExtra in 2022
*/

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "rotozoom.h"

SDL_Texture* screen;		//The texture representing the screen	
SDL_Texture* roto;		//The texture representing the rotozoomed image	
SDL_Surface* source;		//A source image
SDL_Rect src, dest;

Uint32* pixels = NULL;		//The pixel buffer to draw to
int d = 0;
float sin_t[360];

int init_roto(SDL_Renderer* renderer) {

	//set up screen texture
	screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	//set up screen texture
	roto = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, ROTO_WIDTH, ROTO_HEIGHT);
	
	//allocate pixel buffer
	pixels = (Uint32*) malloc((ROTO_WIDTH * ROTO_HEIGHT) * sizeof(Uint32));
	
	//load image into surface
	SDL_Surface* tmp = SDL_LoadBMP(".\\logoA.bmp"); // BMP file with size 100 x 100 px
	
	if (tmp == NULL) { 
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] Could not load image file! SDL_Error: %s", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	source = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGBA8888, 0);
	SDL_FreeSurface(tmp);

	if (roto == NULL) { 
 		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] Texture could not be created! SDL_Error: %s", SDL_GetError());
 		SDL_Quit();
 		return EXIT_FAILURE; 
	}

	if (screen == NULL) { 
 		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] Texture could not be created! SDL_Error: %s", SDL_GetError());
 		SDL_Quit();
 		return EXIT_FAILURE; 
	}
	
	if (pixels == NULL) {
 		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] Error allocating pixel buffer! SDL_Error: %s", SDL_GetError());
 		SDL_Quit();
 		return EXIT_FAILURE; 
	}

	if (source == NULL) {
 		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] Error allocating pixel buffer! SDL_Error: %s", SDL_GetError());
 		SDL_Quit();
 		return EXIT_FAILURE; 
	}

	// Datasheets
	int i = 0;
	for (i = 0; i < 360; i++)
		sin_t[i] =  sin(i * M_PI / 180);
	
	src.x = 0;
	src.y = 0;
	src.w = ROTO_WIDTH;
	src.h = ROTO_HEIGHT;
	
	dest.x = SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2;
	dest.y = SCREEN_HEIGHT / 2 - SCREEN_WIDTH / 2;
	dest.w = SCREEN_WIDTH;
	dest.h = SCREEN_WIDTH;

	return 0;
}

int play_roto(SDL_Renderer* renderer, SDL_Texture * texture, SDL_Rect position) {

		struct vector2d obj_translate = {-ROTO_WIDTH / 2, -ROTO_HEIGHT / 2};
		struct vector2d screen_translate = {ROTO_WIDTH / 2, ROTO_HEIGHT / 2};
		
		d += 1;
		
		float x,y;

		for (x = 0; x < ROTO_WIDTH; x++) {
			
			for (y = 0; y < ROTO_HEIGHT; y++) {
				
				struct vector2d r = {x, y};
				
				add_vector(&r, &obj_translate);
				int index = (d % 360);
				rotate_vector(&r, sin_t[index] * 360);
				add_vector(&r, &screen_translate);
				
				if (r.y < 0)
					r.y = fabs(r.y);
				
				if (r.x < 0)
					r.x = fabs(r.x);

				float u = (float) r.x / ROTO_WIDTH  * fabs(sin_t[index]) * 5;
				float v = (float) r.y / ROTO_HEIGHT * fabs(sin_t[index]) * 5;
				u = (int) (u * source->w) % source->w;
				v = (int) (v * source->h) % source->h;
				
				Uint32 c = get_pixel(u, v);
				draw_pixel(x, y, c);
			}
		}

		SDL_UpdateTexture(roto, NULL, pixels, ROTO_WIDTH * sizeof (Uint32));

		//draw to the screen
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, roto, &src, &dest); // roto
		SDL_RenderCopy(renderer, texture, NULL, &position); // text
		SDL_RenderPresent(renderer);

		return 0;
}
				

int stop_roto() {

	// free the screen buffer memory
	free(pixels);

	// free the source image
    if(NULL != source)
         SDL_FreeSurface(source);

	return 0;
}

void draw_pixel(int x, int y, Uint32 c) {

	Uint32 position = y * ROTO_WIDTH + x;
	pixels[position] = c;
}

Uint32 get_pixel(int x, int y) {

	Uint32* p = (Uint32 *) source->pixels;
	Uint32 position = y * source->w + x;

	return p[position];
}

