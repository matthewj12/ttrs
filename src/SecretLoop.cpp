#include "GlobalVariables.h"
#include "GlobalFunctions.h"
#include "Piece.h"
#include "Tetrominos.h"
#include "Text.h"

#include <SDL_image.h>
#include <SDL_ttf.h>
// #include <SDL2_gfxPrimitives.h>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>


void secretLoop() {
	Text test_text(window_w / 2, 0, "E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E E", 50, &white, 300);
	Text test_text_2(0, 0, "Hello World", 50, &white);

	SDL_Event event;
	while (cur_screen_func_ptr == &secretLoop) {
		// _________________________ EVENTS _________________________
		{
			while (SDL_PollEvent(&event)) {
				processQuitEvents(event);
			}
		}
		//  ________________________ LOGIC __________________________
		{
		
		}
		// ________________________ DRAWING _________________________
		{
			SDL_SetRenderDrawColor(renderer, black[0], black[1], black[2], 255);
			SDL_RenderClear(renderer);

			test_text.draw();
			test_text_2.draw();
			
			SDL_RenderPresent(renderer);
			limitTPS();
		}
	}
}
