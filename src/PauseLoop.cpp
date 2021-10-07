#include "GlobalVariables.h"
#include "GlobalFunctions.h"
#include "Text.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

void gameLoop();
void mainMenuLoop();

enum class options {
	Continue,
	MainMenu,
	OptionsSize
};

void pauseLoop() {
	static SDL_Event event;
	static Text pause_text = Text(0, h_1_top, "Paused", lg_font_size, &white);
	array<Text*, (int) options::OptionsSize> option_texts = {
		new Text(0, button_top, "Continue", med_font_size, &white),
		new Text(0, button_top, "Main Menu", med_font_size, &med_grey)
	};
	pause_text.setCenterX(window_w / 2);
	option_texts[0]->setCenterX(window_w / 2 - grid_cell_w * 6);
	option_texts[1]->setCenterX(window_w / 2 + grid_cell_w * 6);
	
	options selected = options::Continue;

	while (cur_screen_func_ptr == &pauseLoop) {
		while (SDL_PollEvent(&event)) {
			processQuitEvents(event);

			if (event.type == SDL_KEYDOWN && !event.key.repeat) {
				kc key = (kc) event.key.keysym.sym;

				if (isDirectionKey(key)) {
					if (key == kc::Left) {
						option_texts[(int) selected]->setColor(&med_grey);
						if ((int) selected - 1 >= 0) {
							selected = (options) ((int) selected - 1);
						}
						else {
							selected = (options) ((int) options::OptionsSize - 1);
						}
						option_texts[(int) selected]->setColor(&white);
					}
					else if (key == kc::Right) {
						option_texts[(int) selected]->setColor(&med_grey);
						if ((int) selected + 1 < (int) options::OptionsSize) {
							selected = (options) ((int) selected + 1);
						}
						else {
							selected = (options) 0;
						}
						option_texts[(int) selected]->setColor(&white);
					}
				}
				else if (key == kc::Space) {
						if (selected == options::Continue) {
							cur_screen_func_ptr = &gameLoop;
						}
						else if (selected == options::MainMenu) {
							cur_screen_func_ptr = &mainMenuLoop;
						}
					}
			}

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			pause_text.draw();
			
			for (Text* t : option_texts) {
				t->draw();
			}
					
			SDL_RenderPresent(renderer);
			limitTPS();
		}
	}
}