/*
SDL 2 code source for Visual Source Code by Zorro 2^NoExtra in 2022
Original Rotozoom code source from Lazy Foo' Productions

Requisites:
-----------
- VSC :: v1.67.0 64bits
- OS :: Windows_NT x64 10.0.19044
- MinGW-w64 and MSYS2 :: msys2-x86_64-20220128
- SDL2 :: SDL2-2.0.20-win32-x64
- SDL2 MAIN :: SDL2-devel-2.0.20-mingw
- SDL2 TTF :: SDL2_ttf-devel-2.0.18-mingw
- SDL2 IMAGE :: SDL2_image-devel-2.0.5-mingw
- Windows System font file :: C:\Windows\Fonts\BRITANIC.TTF
- Logo Atari (BMP file 100x100 px) file :: logoA.bmp
*/

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

extern DECLSPEC int SDLCALL TTF_Init(void);
#define TTF_GetError    SDL_GetError

#include "vector.h"
#include "rotozoom.h"

SDL_Window * window = NULL;	//The window we'll be rendering to
SDL_Renderer * renderer;		//The renderer SDL will use to draw to the screen

#define WIDTHSCREEN 1080
#define HEIGHTSCREEN 720

SDL_Surface * text;
SDL_Texture * texture;
TTF_Font * font;
SDL_Rect position;
int texW = 0;
int texH = 0;

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    int statut = EXIT_FAILURE;

    // Timer FPS
    float elapsedMS;
    char caption[ 64 ];

	/* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
 
    /* Message Box */
    int success;
    success = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                "Benchmark Launcher",
                "Are you ready for the tests ?",
                NULL);
    if (success == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[ERROR] MessageBox: %s\n", SDL_GetError());
		return statut;
    }

    //Time settings
	int sleep = 0;
	Uint32 next_game_tick = SDL_GetTicks();

    // Starting SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[ERROR] %s", SDL_GetError());
		return statut;
    }

    // Opening a Window
    SDL_Window *window = SDL_CreateWindow("",0,0,0,0,0);
    if(!window){
 		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[ERROR] %s", SDL_GetError());
 		SDL_Quit();
 		return statut;
    }

    // SDL_SetWindowPosition(window,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowPosition(window,50,50);
    SDL_SetWindowSize(window,SCREEN_WIDTH,SCREEN_HEIGHT);
    SDL_SetWindowTitle(window, "Benchmark Launcher.");
    SDL_ShowWindow(window);

    // Creating a Renderer
    // * SDL_RENDERER_SOFTWARE : utilise l'accélération logiciel pour faire les calculs de rendu depuis le CPU
    // * SDL_RENDERER_ACCELERATED : utilise l'accélération matériel pour faire les calculs de rendu depuis la GPU
    // * SDL_RENDERER_PRESENTVSYNC : synchronise l'affichage en fonction du taux de rafraîchissement de votre écran
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer){
 		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[ERROR] %s", SDL_GetError());
 		SDL_Quit();
 		return statut;
    }

    // Starting SDL_ttf
    if(TTF_Init() < 0){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[ERROR] %s", TTF_GetError());
		return statut;
    }

    // Font settings
    TTF_Font * font = TTF_OpenFont("C:/Windows/Fonts/BRITANIC.TTF", 25);
    if(NULL == TTF_GetError()){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[ERROR] %s", TTF_GetError());
		return statut;
    }
    SDL_Color textcolor = { 0, 0, 0 }; // noir

    // Init Roto
    init_roto(renderer);

    //Our event structure
    SDL_Event e;
    bool quit = false;
    while (!quit){

        Uint64 start = SDL_GetPerformanceCounter();

        //check for new events every frame
		SDL_PumpEvents();
		const Uint8 *state = SDL_GetKeyboardState(NULL);

	    while (SDL_PollEvent(&e)){
		    if (e.type == SDL_QUIT){
		    	quit = true;
		    }
		    if (e.type == SDL_KEYDOWN){
		    	quit = true;
		    }
		    if (e.type == SDL_MOUSEBUTTONDOWN){
		    	quit = true;
		    }
	    } // while

        // Text
        text = TTF_RenderText_Blended (font, caption, textcolor);
        texture = SDL_CreateTextureFromSurface(renderer, text);
        // Centre la texture sur l'écran
        position.x = WIDTHSCREEN / 2 - position.w / 2;
        position.y = HEIGHTSCREEN / 2 - position.h / 2;
        SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);

        // Display Rotozoom
        play_roto(renderer, texture, position);

        // Calcul du timing
        Uint64 end = SDL_GetPerformanceCounter();
        // FPS= 1000/Nombre(ms) donc 30fps= 1000/33   50fps=1000/20
        elapsedMS  = floor( 16.666f - ((end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f));
        elapsedMS = 50.0f + elapsedMS;
    
        //On calcule le nombre de frames par seconde et on cree la chaine de caracteres
        sprintf( caption, "Frames Per Seconde: %.0f", elapsedMS );

        // Couleur du texte
        if ( elapsedMS >= 49 ) {
            textcolor.r = 0; // noir
            textcolor.g = 0;
            textcolor.b = 0;
        } // if
        if ( elapsedMS >= 39 && elapsedMS < 49) {
            textcolor.r = 255; // jaune
            textcolor.g = 255;
            textcolor.b = 0;
        } // if
        if ( elapsedMS >= 29 && elapsedMS < 39) {
            textcolor.r = 255; // orange
            textcolor.g = 165;
            textcolor.b = 0;
        } // if
        if ( elapsedMS < 29) {
            textcolor.r = 255; // rouge
            textcolor.g = 0;
            textcolor.b = 0;
        } // if

        // Time it takes to render 1 frame in milliseconds
		next_game_tick += 1000 / 60;
		sleep = next_game_tick - SDL_GetTicks();
		if( sleep >= 0 )
			SDL_Delay(sleep);
    }

    statut = EXIT_SUCCESS;

    stop_roto();

    if(NULL != texture)
        SDL_DestroyTexture(texture);
    if(NULL != text)
         SDL_FreeSurface(text);
    if(NULL != font)
         TTF_CloseFont(font);
    TTF_Quit();

    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();

    return statut;
} // main()