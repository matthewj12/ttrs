#include "GlobalVariables.h"
#include "GlobalFunctions.h"
#include "Text.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

void gameLoop();
void highScoresLoop();
void settingsLoop();

enum class options {
	Settings,
	HighScores,
	OptionsSize
};

void mainMenuLoop() {
	SDL_Event event;
	options selected = options::Settings;

	Text title_text = Text(0, h_1_top, "TetrisEX", lg_font_size);
	Text play_text = Text(0, h_2_top, "Press Enter to Play", med_font_size, &med_grey);
	array<Text*, (int) options::OptionsSize> option_texts = {
		new Text(0, button_top, "Settings", med_font_size, &white),
		new Text(0, button_top, "High Scores", med_font_size, &med_grey)
	};
	title_text.setCenterX(window_w / 2);
	play_text.setCenterX(window_w / 2);
	option_texts[0]->setCenterX(window_w / 2 - grid_cell_w * 6);
	option_texts[1]->setCenterX(window_w / 2 + grid_cell_w * 6);

	option_texts[0]->setColor(&white);
	option_texts[1]->setColor(&med_grey);

	while (cur_screen_func_ptr == &mainMenuLoop) {
		// _________________________ EVENTS _________________________
		{
			while (SDL_PollEvent(&event)) {
				processQuitEvents(event);

				if (event.type == SDL_KEYDOWN && !event.key.repeat) {
					switch ((kc) event.key.keysym.sym) {
						case kc::Right:
							option_texts[(int) selected]->setColor(&med_grey);

							if ((int) selected + 1 >= (int) options::OptionsSize) {
								selected = (options) 0;
							}
							else {
								selected = (options) ((int) selected + 1);
							}
							option_texts[(int) selected]->setColor(&white);
							break;
						case kc::Left:
							option_texts[(int) selected]->setColor(&med_grey);

							if ((int) selected - 1 < 0) {
								selected = (options) ((int) options::OptionsSize - 1);
							}
							else {
								selected = (options) ((int) selected - 1);
							}
							option_texts[(int) selected]->setColor(&white);
							break;
						case kc::Enter:
							cur_screen_func_ptr = &gameLoop;
							break;
						case kc::Space:
							switch (selected) {
								case options::Settings:
									cur_screen_func_ptr = &settingsLoop;
									break;
								case options::HighScores:
									cur_screen_func_ptr = &highScoresLoop;
									break;
							}
							break;
					}
				}
			}
		}
		//  ________________________ LOGIC __________________________
		{

		}

		//  ________________________ DRAWING ________________________
		{
			SDL_SetRenderTarget(renderer, NULL);
			SDL_SetRenderDrawColor(renderer, black[0], black[1], black[2], 255);
			SDL_RenderClear(renderer);

			title_text.draw();
			play_text.draw();

			for (Text* t : option_texts) {
				t->draw();
			}

			SDL_RenderPresent(renderer);
			limitTPS();
		}
	}
}