#include "GlobalVariables.h"
#include "GlobalFunctions.h"
#include "Text.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

void gameLoop();
void mainMenuLoop();

enum class options {
	SettingsPreset,
	PieceGenMode,
	ScoringMode,
	LevelProgressionMode,
	PieceHoldOn,
	PiecePreviews,
	Size
};

int i1 = 1;
int i2 = 2;
int i3 = 3;
int i4 = 6;
int i5 = 8;

void createHighScoreTexts(array<Text*, 5>& p_high_score_texts, array<int, (int) options::Size>* selected_options_ptr) {
	Settings temp_settings = Settings(0, (*selected_options_ptr)[1], (*selected_options_ptr)[2], (*selected_options_ptr)[3], 0, 0, (*selected_options_ptr)[4], 0, (*selected_options_ptr)[5]);

	for (int i = 0; i < 5; i++) {
		p_high_score_texts[i] = new Text(0, h_4_top + i * grid_cell_w * 1.4, to_string(save_data.m_SS_map[cur_settings_ptr->hash()][i]), med_font_size, &white, -1, jst::R);
		p_high_score_texts[i]->setRight(window_w / 2 - grid_cell_w * 4);
	}
	for (int i = 0; i < 5; i++) {
		if (save_data.m_SS_map.find(temp_settings.hash()) != save_data.m_SS_map.end()) {
			p_high_score_texts[i] = new Text(0, h_4_top + i * grid_cell_w * 1.4, to_string(save_data.m_SS_map[cur_settings_ptr->hash()][i]), med_font_size, &white, -1, jst::R);
		}
		else {
			p_high_score_texts[i] = new Text(0, h_4_top + i * grid_cell_w * 1.4, i == 0 ? "No Data" : "", med_font_size, &white, -1, jst::R);
		}
		p_high_score_texts[i]->setRight(window_w / 2 - grid_cell_w * 4);
	}
}

void updateHighScoreTexts(array<Text*, 5>& p_high_score_texts, array<int, (int) options::Size>* selected_options_ptr) {
	Settings temp_settings = Settings(0, (*selected_options_ptr)[1], (*selected_options_ptr)[2], (*selected_options_ptr)[3], 0, 0, (*selected_options_ptr)[4], 0, (*selected_options_ptr)[5]);

	for (int i = 0; i < 5; i++) {
		if (save_data.m_SS_map.find(temp_settings.hash()) != save_data.m_SS_map.end()) {
			p_high_score_texts[i]->setText(to_string(save_data.m_SS_map[temp_settings.hash()][i]));
		}
		else {
			p_high_score_texts[i]->setText(i == 0 ? "No Data" : "");
		}
	}
}

void createOptionTexts(
	options p_selected, 
	vector<vector<string>>* p_option_strings_ptr,
	array<string, (int) options::Size>* p_option_name_strings_ptr,
	array<Text*, (int) options::Size>* p_option_texts_ptr,
	array<Text*, (int) options::Size>* p_option_name_texts_ptr,
	array<int, (int) options::Size>* p_selected_options_ptr
) {
	int cur_y = h_4_top;

	for (int i = 0; i < (int) options::Size; i++) {
		(*p_option_name_texts_ptr)[i] = new Text(window_w / 2 + grid_cell_w * 6, cur_y, (*p_option_name_strings_ptr)[i], sm_font_size, &green, -1, jst::R);

		(*p_option_texts_ptr)[i] = new Text(0, cur_y, (*p_option_strings_ptr)[i][(*p_selected_options_ptr)[i]], sm_font_size, (int) p_selected == i ? &white : &med_grey, -1, jst::R);

		(*p_option_texts_ptr)[i]->setRight(window_w / 2 + grid_cell_w * 18);
		cur_y += grid_cell_w * 1.4;
	}
}

void restrainSelectablePiecePreviews(
	vector<vector<string>>* p_option_strings_ptr, 
	array<int, (int) options::Size>* p_selected_options_ptr, 
	array<Text*, (int) options::Size>* p_option_texts_ptr
) {
	int i3 = (int) options::PiecePreviews;
	int i4 = (int) options::PieceGenMode;

	(*p_option_strings_ptr)[i3] = *(selectable_piece_previews[(piece_gen_modes) (*p_selected_options_ptr)[i4]]);

	if ((*p_selected_options_ptr)[i3] >= (*p_option_strings_ptr)[i3].size()) {
		(*p_selected_options_ptr)[i3] = (*p_option_strings_ptr)[i3].size()-1;
	}
	(*p_option_texts_ptr)[i3]->setText((*p_option_strings_ptr)[i3][(*p_selected_options_ptr)[i3]]);
}

void highScoresLoop() {
	SDL_Event event;

	static array<string, (int) options::Size>* option_name_strings_ptr = new array<string, (int) options::Size> {
		"Settings Preset",
		"Piece Generation Mode",
		"Scoring Mode",
		"Level Progression Mode",
		"Piece Hold",
		"Piece Previews"
	};

	static vector<vector<string>>* option_strings_ptr = new vector<vector<string>> {{
		{"NES", "Modern", "Custom"},
		{"NES", "Bag", "True Random", "Custom"},
		{"NES/GameBoy/SNES", "Tetris 4D", "Modern"},
		{"NES/GameBoy/SNES", "Every 10 Lines", "No Progression", "Custom"},
		{"Disabled", "Enabled"},
		*(selectable_piece_previews[cur_settings_ptr->m_piece_gen_mode])
	}};

	options selected = options::SettingsPreset;
	array<int, 9> p = presets[(int) settings_presets::NES];
	array<int, (int) options::Size>* selected_options_ptr = new array<int, (int) options::Size>{0, p[i1], p[i2], p[i3], p[i4], p[i5]};

	array<Text*, (int) options::Size>* option_texts_ptr = new array<Text*, (int) options::Size>;
	static array<Text*, (int) options::Size>* option_name_texts_ptr = new array<Text*, (int) options::Size>;

	createOptionTexts(selected, option_strings_ptr, option_name_strings_ptr, option_texts_ptr, option_name_texts_ptr, selected_options_ptr);

	static Text title_text  = Text(0, h_1_top, "High Scores", lg_font_size, &white);
	static Text play_text   = Text(0, h_2_top, "Press Enter to Play", med_font_size, &med_grey);
	static Text return_text = Text(0, h_5_top, "Press Shift to Go Back", med_font_size, &med_grey);

	array<Text*, 5> high_score_texts;
	createHighScoreTexts(high_score_texts, selected_options_ptr);
	
	title_text.setCenterX(window_w / 2);
	play_text.setCenterX(window_w / 2);
	return_text.setCenterX(window_w / 2);
	
	while (cur_screen_func_ptr == &highScoresLoop) {
		// _________________________ EVENTS _________________________
		{
			while (SDL_PollEvent(&event)) {
				processQuitEvents(event);

				if (event.type == SDL_KEYDOWN && !event.key.repeat) {
					kc key = (kc) event.key.keysym.sym;
					if (key == kc::Enter) {
						cur_screen_func_ptr = &gameLoop;
					}
					
					else if (key == kc::Shift) {
						cur_screen_func_ptr = &mainMenuLoop;
					}
					
					if (key == kc::Down) {
						(*option_texts_ptr)[(int) selected]->setColor(&med_grey);

						if ((int) (selected) + 1 < (int) options::Size) {
							selected = (options) (((int) selected) + 1);
						}
						else {
							selected = (options) 0;
						}
						(*option_texts_ptr)[(int) selected]->setColor(&white);
					}
					
					else if (key == kc::Up) {
						(*option_texts_ptr)[(int) selected]->setColor(&med_grey);

						if ((int) (selected) - 1 >= 0) {
							selected = (options) (((int) selected) - 1);
						}
						else {
							selected = (options) (((int) options::Size) - 1);
						}
						(*option_texts_ptr)[(int) selected]->setColor(&white);
					}

					else if (key == kc::Right || key == kc::Left) {
						if (key == kc::Right) {
							if ((*selected_options_ptr)[(int) selected] + 1 >= (*option_strings_ptr)[(int) selected].size()) {
								(*selected_options_ptr)[(int) selected] = 0;
							}
							else {
								(*selected_options_ptr)[(int) selected] += 1;
							}
						}

						else if (key == kc::Left) {
							if ((*selected_options_ptr)[(int) selected] - 1 < 0) {
								(*selected_options_ptr)[(int) selected] = (*option_strings_ptr)[(int) selected].size() - 1;
							}
							else {
								(*selected_options_ptr)[(int) selected] -= 1;
							}
						}

						(*option_texts_ptr)[(int) selected]->setText((*option_strings_ptr)[(int) selected][(*selected_options_ptr)[(int) selected]]);

						if (selected == options::PieceGenMode) {
							restrainSelectablePiecePreviews(option_strings_ptr, selected_options_ptr, option_texts_ptr);
						}
						
						if (selected == options::SettingsPreset) {
							if ((*selected_options_ptr)[(int) selected] != (*option_strings_ptr)[(int) options::SettingsPreset].size() - 1) {
								array<int, 9> p = presets[(*selected_options_ptr)[(int) selected]];

								(*selected_options_ptr)[1] = p[i1];
								(*selected_options_ptr)[2] = p[i2];
								(*selected_options_ptr)[3] = p[i3];
								(*selected_options_ptr)[4] = p[i4];
								(*selected_options_ptr)[5] = p[i5];

								restrainSelectablePiecePreviews(option_strings_ptr, selected_options_ptr, option_texts_ptr);
								
								for (int i = 0; i < option_texts_ptr->size(); i++) {
									(*option_texts_ptr)[i]->setText((*option_strings_ptr)[i][(*selected_options_ptr)[i]]);
								}
							}
						}
						
						else {
							int sp = (int) options::SettingsPreset;
							bool preset_found = false;
							array<int, (int) options::Size - 1> relevant_options;

							for (int i = 1; i < selected_options_ptr->size(); i++) {
								relevant_options[i-1] = (*selected_options_ptr)[i];
							}
							// if "Custom" is the currently selected preset
							if ((*selected_options_ptr)[sp] == (*option_strings_ptr)[sp].size() - 1) {
								// Check current settings configuration against presets
								for (int i = 0; i < presets.size(); i++) {
									if (presets[i][i1] == relevant_options[0] && presets[i][i2] == relevant_options[1] && presets[i][i3] == relevant_options[2] && presets[i][i4] == relevant_options[3] && presets[i][i5] == relevant_options[4]) {
										(*selected_options_ptr)[sp] = i;
										(*option_texts_ptr)[sp]->setText((*option_strings_ptr)[sp][(*selected_options_ptr)[sp]]);
										preset_found = true;
										break;
									}
								}
							}
							else if (!preset_found) {
								(*selected_options_ptr)[sp] = (*option_strings_ptr)[sp].size() - 1;
								(*option_texts_ptr)[sp]->setText((*option_strings_ptr)[sp][(*selected_options_ptr)[sp]]);
							}
						}

						updateHighScoreTexts(high_score_texts, selected_options_ptr);

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

			title_text.draw();
			play_text.draw();
			return_text.draw();

			for (Text* t : high_score_texts) {
				t->draw();
			}

			for (Text* t : *option_texts_ptr) {
				t->draw();
			}

			for (Text* t : *option_name_texts_ptr) {
				t->draw();
			}

			SDL_RenderPresent(renderer);
			limitTPS();
		}
	}
}
