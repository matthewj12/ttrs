#include "GlobalVariables.h"
#include "GlobalFunctions.h"
#include "Text.h"
#include "Piece.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <math.h>

void gameLoop();
void settingsLoop();

void drawPieceSeq(int p_selected_piece, int p_seq_left, int p_box_w, int p_space_between_boxes, int p_y) {
	for (int i = 0; i < custom_piece_seq.size(); i++) {
		// piece->draw();
		drawBox(p_seq_left + i * p_space_between_boxes, 
				p_y, 
				p_box_w, 
				p_box_w, 
				3, i == p_selected_piece ? &white : &drk_grey);
	}
}

void drawProbsPieces(int p_selected_prob, int p_seq_left, int p_seq_top, int p_box_w, int p_space_between_boxes, bool p_selecting_probs) {
	for (int i = 0; i < 7; i++) {
		drawBox(p_seq_left + i * p_space_between_boxes, 
				p_seq_top, 
				p_box_w, 
				p_box_w, 
				3, p_selecting_probs ? (i == p_selected_prob ? &white : &drk_grey) : &drk_grey);

		for (int row = 0; row < 4; ++row) {
			for (int col = 0; col < 4; col++) {
				if ((char) tetrominos[i][0][row][col] != EMPTY_CELL) {
					SDL_Rect r;
					r.x = p_seq_left + i * p_space_between_boxes + col * grid_cell_w;
					r.y = p_seq_top + row * grid_cell_w;
					r.w = grid_cell_w;
					r.h = grid_cell_w;
					array<Uint8, 3> color = GCCM[tetrominos[i][0][2][2]];
					
					SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);
					SDL_RenderFillRect(renderer, &r);
				}		
			}
		}
	}
}

void drawPlus(int p_selected_piece, SDL_Point p_plus_points_ptr[13]) {
	if (p_selected_piece == custom_piece_seq.size()) {
		SDL_SetRenderDrawColor(renderer, white[0], white[1], white[2], 255);
	}
	else {
		SDL_SetRenderDrawColor(renderer, drk_grey[0], drk_grey[1], drk_grey[2], 255);
	}
	SDL_RenderDrawLines(renderer, p_plus_points_ptr, 13);
}

void updatePlusPoints(SDL_Point p_plus_points_ptr[13], bool p_piece_added, int p_space_between_boxes) {
	int to_add = p_piece_added ? p_space_between_boxes : -p_space_between_boxes;

	for (auto it = p_plus_points_ptr; it < p_plus_points_ptr + 13; it++) {
		it->x += to_add; 
	}
}

void updateProbTexts(array<Text*, 7>* p_prob_texts_ptr, int p_probs_left, int p_space_between_boxes, int p_selected_piece) {
	for (int i = 0; i < 7; ++i) {
		(*p_prob_texts_ptr)[i]->setText(to_string(custom_piece_seq[p_selected_piece][i]) + " %");
	}
}

void customPieceSeqLoop() {
	int box_w = grid_cell_w * 4;
	int space_between_boxes = grid_cell_w * 5;
	int seq_left = grid_cell_w * 1;
	int probs_left = window_w / 2 - 3.5 * space_between_boxes;	
	int selected_piece = 0;
	int selected_prob = 0; // Selected probability for selected piece
	bool selecting_probs = false; // selection is in the probability line

	int row_1 = window_h / 2 - grid_cell_w;
	int row_2 = window_h / 2 + grid_cell_w * 5;

	array<Text*, 7>* prob_texts_ptr = new array<Text*, 7>;
	for (int i = 0; i < 7; ++i) {
		(*prob_texts_ptr)[i] = new Text(probs_left + i * space_between_boxes, row_2, "", med_font_size, &med_grey);
	}

	int start_x = seq_left + custom_piece_seq.size() * space_between_boxes;
	static SDL_Point plus_points_ptr[13] = {
		{start_x,                     row_1 + box_w * (2.0/3.0)},
		{start_x,                     row_1 + box_w * (1.0/3.0)},
		{start_x + box_w * (1.0/3.0), row_1 + box_w * (1.0/3.0)},
		{start_x + box_w * (1.0/3.0), row_1},
		{start_x + box_w * (2.0/3.0), row_1},
		{start_x + box_w * (2.0/3.0), row_1 + box_w * (1.0/3.0)},
		{start_x + box_w,             row_1 + box_w * (1.0/3.0)},
		{start_x + box_w,             row_1 + box_w * (2.0/3.0)},
		{start_x + box_w * (2.0/3.0), row_1 + box_w * (2.0/3.0)},
		{start_x + box_w * (2.0/3.0), row_1 + box_w},
		{start_x + box_w * (1.0/3.0), row_1 + box_w},
		{start_x + box_w * (1.0/3.0), row_1 + box_w * (2.0/3.0)},
		{start_x,                     row_1 + box_w * (2.0/3.0)}
	};

	static Text title_text = Text(0, h_1_top, "Custom Piece Generator", lg_font_size, &white);
	static Text play_text = Text(0, h_2_top, "Press Enter to Play", med_font_size, &med_grey);
	static Text return_text = Text(0, h_5_top, "Press Shift to Go Back", med_font_size, &med_grey);

	title_text.setCenterX(window_w / 2);
	play_text.setCenterX(window_w / 2);
	return_text.setCenterX(window_w / 2);

	SDL_Event event;

	while (cur_screen_func_ptr == &customPieceSeqLoop) {
		// _________________________ EVENTS _________________________
		{
			while (SDL_PollEvent(&event)) {
				processQuitEvents(event);

				if (event.type == SDL_KEYDOWN && !event.key.repeat) {
					kc key = (kc) event.key.keysym.sym;

					if (key == kc::Up || key == kc::Down) {
						if (selecting_probs && !custom_piece_seq.empty() && selected_piece < custom_piece_seq.size()) {
							custom_piece_seq[selected_piece][selected_prob] += key == kc::Up ? (key_states[(int) sc::Ctrl] ? 5 : 10) : (key_states[(int) sc::Ctrl] ? -5: -10);

							if (custom_piece_seq[selected_piece][selected_prob] > 100) {
								custom_piece_seq[selected_piece][selected_prob] = 0;
							}
							else if (custom_piece_seq[selected_piece][selected_prob] < 0) {
								custom_piece_seq[selected_piece][selected_prob] = 100;
							}
							(*prob_texts_ptr)[selected_prob]->setText(to_string((custom_piece_seq[selected_piece][selected_prob])) + " %");
						}
					}
					else if (key == kc::Left || key == kc::Right) {
						if (selecting_probs) {
							selected_prob += key == kc::Left ? -1 : 1;
							if (selected_prob > 6) { selected_prob = 0; }
							else if (selected_prob < 0) { selected_prob = 6; }
						}
						else {
							selected_piece += key == kc::Left ? -1 : 1;
							if (selected_piece > (int) custom_piece_seq.size()) {
								selected_piece = 0;
							}
							if (selected_piece < 0) {
								selected_piece = custom_piece_seq.size();
							}

							updateProbTexts(prob_texts_ptr, probs_left, space_between_boxes, selected_piece);
						}
					}
					else if (key == kc::Enter) {
						cur_screen_func_ptr = &gameLoop;
					}
					else if (key == kc::Shift) {
						if (selecting_probs) {
							selecting_probs = false;
						}
						else {
							cur_screen_func_ptr =  &settingsLoop;
						}
					}
					else if (key == kc::Space) {
						if (selected_piece == custom_piece_seq.size()) { // "+" is selected_piece
							custom_piece_seq.push_back({0,0,0,0,0,0,0});
							// prob_texts_ptr->push_back(default_prob_texts);
							updateProbTexts(prob_texts_ptr, probs_left, space_between_boxes, selected_piece);

							updatePlusPoints(plus_points_ptr, true, space_between_boxes);
						}
						else if (!selecting_probs) {
							selecting_probs = true;
						}
					}
					else if (key == kc::x) {
						if (selected_piece < custom_piece_seq.size()) {
							custom_piece_seq.erase(custom_piece_seq.begin() + selected_piece);
							selecting_probs = false;

							updatePlusPoints(plus_points_ptr, false, space_between_boxes);
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

			title_text.draw();
			play_text.draw();
			return_text.draw();

			drawPieceSeq(selected_piece, seq_left, box_w, space_between_boxes, row_1);
			drawPlus(selected_piece, plus_points_ptr);

			if (!custom_piece_seq.empty() && selected_piece < custom_piece_seq.size()) {
				drawProbsPieces(selected_prob, probs_left, row_2, box_w, space_between_boxes, selecting_probs);
				for (Text* t : *prob_texts_ptr) {
					t->draw();
				}
			}
			

			SDL_RenderPresent(renderer);
			limitTPS();
		}
	}
}
