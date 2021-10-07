#include "GlobalVariables.h"
#include "GlobalFunctions.h"
#include "Text.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

void gameLoop();
void mainMenuLoop();
void customPieceSeqLoop();
void customLevelProgLoop();

enum class options {
	Level,
	SettingsPreset,
	MovementMode,
	PieceGenMode,
	ScoringMode,
	LevelProgressionMode,
	PieceStartHeight,
	WallKickOn,
	PieceHoldOn,
	PiecePreviewOn,
	PiecePreviews,
	Size
};

void shiftOptionTexts(int p_x_offset, int p_y_offset, int p_space_between_options, 
	array<Text*, (int) options::Size>* p_option_texts_ptr, 
	array<Text*, (int) options::Size>* p_option_name_texts_ptr) 
{
	for (int i = 0; i < p_option_texts_ptr->size(); i++) {
		(*p_option_texts_ptr)[i]->shiftX(p_x_offset);
		(*p_option_texts_ptr)[i]->shiftY(p_y_offset);
		(*p_option_name_texts_ptr)[i]->shiftX(p_x_offset);
		(*p_option_name_texts_ptr)[i]->shiftY(p_y_offset);
	}
}

void restrainSelectableLevels(
	vector<vector<string>>* p_option_strings_ptr, 
	vector<string>* p_old_selectable_levels_ptr, 
	array<int, (int) options::Size>* p_selected_options_ptr, 
	array<Text*, (int) options::Size>* p_option_texts_ptr
) {
	int i1 = (int) options::Level;
	int i2 = (int) options::ScoringMode;
	(*p_option_strings_ptr)[i1] = *selectable_levels[(scoring_modes) (*p_selected_options_ptr)[i2]];
	// position of identical level value in new selectable level vector
	auto new_pos = find((*p_option_strings_ptr)[i1].begin(), (*p_option_strings_ptr)[i1].end(), (*p_old_selectable_levels_ptr)[(*p_selected_options_ptr)[i1]]);

	if (new_pos != (*p_option_strings_ptr)[i1].end()) {
		(*p_selected_options_ptr)[i1] = distance((*p_option_strings_ptr)[i1].begin(), new_pos);
	}
	else {
		if (stoi((*p_option_strings_ptr)[i1][0]) > stoi((*p_old_selectable_levels_ptr)[0])) {
			(*p_selected_options_ptr)[i1] = 0;
		}
		else {
			(*p_selected_options_ptr)[i1] = (*p_option_strings_ptr)[i1].size() - 1;
		}
	}
	
	(*p_option_texts_ptr)[i1]->setText((*p_option_strings_ptr)[i1][(*p_selected_options_ptr)[i1]]);
	p_old_selectable_levels_ptr = &((*p_option_strings_ptr)[i1]);
}

void restrainSelectablePiecePreviews(
	vector<vector<string>>* p_option_strings_ptr, 
	vector<string>* p_old_selectable_levels_ptr, 
	array<int, (int) options::Size>* p_selected_options_ptr, 
	array<Text*, (int) options::Size>* p_option_texts_ptr
) {
	int i1 = (int) options::PiecePreviews;
	int i2 = (int) options::PieceGenMode;

	(*p_option_strings_ptr)[i1] = *(selectable_piece_previews[(piece_gen_modes) (*p_selected_options_ptr)[i2]]);

	if ((*p_selected_options_ptr)[i1] >= (*p_option_strings_ptr)[i1].size()) {
		(*p_selected_options_ptr)[i1] = (*p_option_strings_ptr)[i1].size()-1;
	}
	(*p_option_texts_ptr)[i1]->setText((*p_option_strings_ptr)[i1][(*p_selected_options_ptr)[i1]]);
}

void createOptionTexts(
	options p_selected, 
	vector<vector<string>>* p_option_strings_ptr,
	array<string, (int) options::Size>* p_option_name_strings_ptr,
	array<Text*, (int) options::Size>* p_option_texts_ptr,
	array<Text*, (int) options::Size>* p_option_name_texts_ptr,
	array<int, (int) options::Size>* p_selected_options_ptr,
	int p_cur_x,
	int p_space_between_options
) {
	for (int i = 0; i < (int) options::Size; i++) {
		(*p_option_name_texts_ptr)[i] = new Text(0, h_3_top, (*p_option_name_strings_ptr)[i], med_font_size, &green, p_space_between_options - grid_cell_w);
		(*p_option_name_texts_ptr)[i]->setCenterX(p_cur_x);


		(*p_option_texts_ptr)[i] = new Text(0, h_4_top, (*p_option_strings_ptr)[i][(*p_selected_options_ptr)[i]], med_font_size, ((int) p_selected) == i ? &white : &med_grey, p_space_between_options);
		(*p_option_texts_ptr)[i]->setCenterX(p_cur_x);

		p_cur_x += p_space_between_options;
	}

}

void settingsLoop() {
	static array<string, (int) options::Size>* option_name_strings_ptr = new array<string, (int) options::Size> {
		"Level",
		"Settings Preset",
		"Movement Mode",
		"Piece Generation Mode",
		"Scoring Mode",
		"Level Progression Mode",
		"Piece Starting Height",
		"Wall Kick",
		"Piece Hold",
		"Piece Preview",
		"Piece Previews",
	};

	static vector<vector<string>>* option_strings_ptr = new vector<vector<string>> {{
		*(selectable_levels[cur_settings_ptr->m_scoring_mode]),
		{"NES", "Modern", "Custom"},
		{"Old", "Modern"},
		{"NES", "Bag", "True Random", "Custom"},
		{"NES/GameBoy/SNES", "Tetris 4D", "Modern"},
		{"NES/GameBoy/SNES", "Every 10 Lines", "No Progression", "Custom"},
		{"0", "1", "2", "3", "4", "5"},
		{"Disabled", "Enabled"},
		{"Disabled", "Enabled"},
		{"Disabled", "Enabled"},
		*(selectable_piece_previews[cur_settings_ptr->m_piece_gen_mode]),
	}};
	static vector<vector<string>>* option_description_strings_ptr = new vector<vector<string>> {{
		{"The level to start on"},
		{"Released in 1900", "Released in 2100", "Current configuration does not match a defined preset"},
		{"Pressing a key while already holding a key down negates both actions", "Pressing a key while already holding a key down negates the previously held key"},
		{"Uniform random distribution with bias against repeated pieces (if the randomly chosen piece is the same as the previous piece, a random piece will be chosen again.", "Gives all 7 pieces but in a random order", "Random uniform distribution", "Press space to customize"},
		{"Awards disproportionately more points for clearling more rows simultaneously, also multiplies points awarded by the current level", "Identical to NES/GameBoy/SNES except points are doubled", "Awards disproportionately more points for clearling more rows simultaneously, multiplies points awarded by the current level, and awards points for combos and spins"},
		{"The first level increase takes the most line clears (exact number depends on the level), all subsequent increses happen every ten lines", "The level increases every 10 lines", "The level never increases", "Press space to customize"},
		{"No pieces may be held", "Allows one piece to be held"},
		{"Distance from the top of the grid to spawn piece"},
		{"Moves piece if necessary for rotation"},
		{"Enable or disable a preview of the next [Piece Previews] number of pieces"},
		{"How many pieces to show if Piece Preview is enabled"}
	}};

	options selected = options::Level;
	array<int, (int) options::Size - 2> p = presets[(int) settings_presets::NES];
	static array<int, (int) options::Size>* selected_options_ptr = new array<int, (int) options::Size> {0, 0, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8]};
	static array<Text*, (int) options::Size>* option_texts_ptr = new array<Text*, (int) options::Size>;
	static array<Text*, (int) options::Size>* option_name_texts_ptr = new array<Text*, (int) options::Size>;

	int space_between_options = 300;

	createOptionTexts(selected, option_strings_ptr, option_name_strings_ptr, option_texts_ptr, option_name_texts_ptr, selected_options_ptr, window_w / 2, space_between_options);

	vector<string>* old_selectable_levels_ptr = &((*option_strings_ptr)[0]);

	static Text title_text(0, h_1_top, "Settings", lg_font_size);
	static Text play_text(0, h_2_top, "Press Enter to Play", med_font_size, &med_grey);
	static Text return_text(0, h_5_top, "Press Shift to Go Back", med_font_size, &med_grey);
	static Text option_description_text(0, h_6_top, (*option_description_strings_ptr)[(int) selected][(*selected_options_ptr)[(int) selected]], sm_font_size, &med_grey, window_w / 2);

	option_description_text.setCenterX(window_w / 2);
	title_text.setCenterX(window_w / 2);
	play_text.setCenterX(window_w / 2);
	return_text.setCenterX(window_w / 2);

	cur_settings_ptr = new Settings(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8]);

	static SDL_Event event;

	while (cur_screen_func_ptr == &settingsLoop) {
		// _________________________ EVENTS _________________________
		{
			while (SDL_PollEvent(&event)) {
				processQuitEvents(event);

				if (event.type == SDL_KEYDOWN && !event.key.repeat) {
					kc key = (kc) event.key.keysym.sym;

					if (key == kc::Right) {
						(*option_texts_ptr)[(int) selected]->setColor(&med_grey);

						if ((int) (selected) + 1 < (int) options::Size) {
							selected = (options) (((int) selected) + 1);
							shiftOptionTexts(-space_between_options, 0, space_between_options, option_texts_ptr, option_name_texts_ptr);
						}
						else {
							selected = (options) 0;
							shiftOptionTexts(space_between_options * (option_texts_ptr->size() - 1), 0, space_between_options, option_texts_ptr, option_name_texts_ptr);
						}
						(*option_texts_ptr)[(int) selected]->setColor(&white);
						option_description_text.setText((*option_description_strings_ptr)[(int) selected][min((*selected_options_ptr)[(int) selected], (int) (*option_description_strings_ptr)[(int) selected].size()-1)]);
						
					}

					else if (key == kc::Left) {
						(*option_texts_ptr)[(int) selected]->setColor(&med_grey);

						if ((int) (selected) - 1 >= 0) {
							selected = (options) (((int) selected) - 1);
							shiftOptionTexts(space_between_options, 0, space_between_options, option_texts_ptr, option_name_texts_ptr);
						}
						else {
							selected = (options) (((int) options::Size) - 1);
							shiftOptionTexts(-space_between_options * (option_texts_ptr->size() - 1), 0, space_between_options, option_texts_ptr, option_name_texts_ptr);
						}
						(*option_texts_ptr)[(int) selected]->setColor(&white);
						option_description_text.setText((*option_description_strings_ptr)[(int) selected][min((*selected_options_ptr)[(int) selected], (int) (*option_description_strings_ptr)[(int) selected].size()-1)]);
					}
					
					else if (key == kc::Up || key == kc::Down) {
						if (key_states[(int) sc::Ctrl]) {
							// hey
						}
						else {
							if (key == kc::Up) {
								if ((*selected_options_ptr)[(int) selected] + 1 >= (*option_strings_ptr)[(int) selected].size()) {
									(*selected_options_ptr)[(int) selected] = 0;
								}
								else {
									(*selected_options_ptr)[(int) selected] += 1;
								}
							}
							
							else {
								if ((*selected_options_ptr)[(int) selected] - 1 < 0) {
									(*selected_options_ptr)[(int) selected] = (*option_strings_ptr)[(int) selected].size() - 1;
								}
								else {
									(*selected_options_ptr)[(int) selected] -= 1;
								}
							}

							(*option_texts_ptr)[(int) selected]->setText((*option_strings_ptr)[(int) selected][(*selected_options_ptr)[(int) selected]]);
							option_description_text.setText((*option_description_strings_ptr)[(int) selected][min((*selected_options_ptr)[(int) selected], (int) (*option_description_strings_ptr)[(int) selected].size()-1)]);

							

							if (selected == options::ScoringMode) {
								restrainSelectableLevels(option_strings_ptr, old_selectable_levels_ptr, selected_options_ptr, option_texts_ptr);
							}
							
							else if (selected == options::PieceGenMode) {
								restrainSelectablePiecePreviews(option_strings_ptr, old_selectable_levels_ptr, selected_options_ptr, option_texts_ptr);
							}
							
							if (selected == options::SettingsPreset) {
								if ((*selected_options_ptr)[(int) selected] != (*option_strings_ptr)[(int) options::SettingsPreset].size() - 1) {
									int level = (*selected_options_ptr)[(int) options::Level];
									
									array<int, (int) options::Size - 2> p = presets[(*selected_options_ptr)[(int) selected]];

									copy(p.begin(), p.end(), selected_options_ptr->begin() + 2);

									(*selected_options_ptr)[(int) options::Level] = level;

									restrainSelectableLevels(option_strings_ptr, old_selectable_levels_ptr, selected_options_ptr, option_texts_ptr);
									restrainSelectablePiecePreviews(option_strings_ptr, old_selectable_levels_ptr, selected_options_ptr, option_texts_ptr);
								
									for (int i = 0; i < option_texts_ptr->size(); i++) {
										(*option_texts_ptr)[i]->setText((*option_strings_ptr)[i][(*selected_options_ptr)[i]]);
										
									}
								}
							}
							
							else {
								bool preset_found = false;
								int sp = (int) options::SettingsPreset;
								array<int, (int) options::Size - 2> relevant_options;

								for (int i = 2; i < selected_options_ptr->size(); i++) {
									relevant_options[i-2] = (*selected_options_ptr)[i];
								}
								
								// Check current settings configuration against presets
								for (int i = 0; i < presets.size(); i++) {
									if (presets[i] == relevant_options) {
										(*selected_options_ptr)[sp] = i;
										(*option_texts_ptr)[sp]->setText((*option_strings_ptr)[sp][(*selected_options_ptr)[sp]]);
										preset_found = true;
										break;
									}
								}
								if (!preset_found) {
									(*selected_options_ptr)[sp] = (*option_strings_ptr)[sp].size() - 1;
									(*option_texts_ptr)[sp]->setText((*option_strings_ptr)[sp][(*selected_options_ptr)[sp]]);
								}
							}
						}
					}

					else if (key == kc::Enter || key == kc::Shift) {
						start_level = (*selected_options_ptr)[0];
						cur_settings_ptr = new Settings(
							(*selected_options_ptr)[2], 
							(*selected_options_ptr)[3], 
							(*selected_options_ptr)[4], 
							(*selected_options_ptr)[5], 
							(*selected_options_ptr)[6], 
							(*selected_options_ptr)[7], 
							(*selected_options_ptr)[8], 
							(*selected_options_ptr)[9], 
							(*selected_options_ptr)[10]);

						if (key == kc::Enter) {
							cout << "Hi" << endl;
							SDL_FlushEvent(SDL_KEYDOWN);
							SDL_FlushEvent(SDL_KEYUP);

							cur_screen_func_ptr = &gameLoop;
						}
						else if (key == kc::Shift) {
							cur_screen_func_ptr = &mainMenuLoop;
						}

						return;
					}
					
					else if (key == kc::Space) {
						if (selected == options::PieceGenMode && (*option_strings_ptr)[(int) selected][(*selected_options_ptr)[(int) selected]] == "Custom") {
							cur_screen_func_ptr = &customPieceSeqLoop;
						}
						if (selected == options::LevelProgressionMode && (*option_strings_ptr)[(int) selected][(*selected_options_ptr)[(int) selected]] == "Custom") {
							cur_screen_func_ptr = &customLevelProgLoop;
						}
					}
				}
			}
		}
		//  ________________________ LOGIC __________________________
		{
			
		}

		//  ________________________ DRAWING ________________________
		{
			SDL_SetRenderDrawColor(renderer, black[0], black[1], black[2], 255);
			SDL_RenderClear(renderer);

			// switch (selected) {
			// 	case option::Level:
			// 		border_rect = level_text_rect;
			// 		border_rect.x -= border_width;
			// 		border_rect.y -= border_width;
			// 		border_rect.w += 2 * border_width;
			// 		border_rect.h += 2 * border_width;
			// 		break;
			// }

			// SDL_SetRenderDrawColor(renderer, green[0], green[1], green[2], 255);
			// SDL_RenderFillRect(renderer, &border_rect);

			title_text.draw();
			play_text.draw();
			return_text.draw();

			for (Text* t : *option_name_texts_ptr) {
				t->draw();
			}
			for (Text* t : *option_texts_ptr) {
				t->draw();
			}

			option_description_text.draw();		

			SDL_RenderPresent(renderer);
			limitTPS();
		}
	}
}
