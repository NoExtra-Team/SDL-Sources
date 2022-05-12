/*
This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.

Also modified for VSC by Zorro 2^NoExtra in 2022
*/

#ifndef ROTOZOOM_H
#define ROTOZOOM_H

//Screen dimension constants
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720
#define ROTO_WIDTH 256
#define ROTO_HEIGHT 256

int init_roto(SDL_Renderer* renderer);
int play_roto(SDL_Renderer* renderer, SDL_Texture * texture, SDL_Rect position);
int stop_roto();
void draw_pixel(int x, int y, Uint32);
Uint32 get_pixel(int x, int y);

#endif //ROTOZOOM_H