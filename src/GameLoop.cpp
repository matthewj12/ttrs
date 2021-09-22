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

void pauseLoop();
void mainMenuLoop();

void buildPlayField(pf_t* p_pf_ptr) {
	*p_pf_ptr = vector<string> {};
	for (int row = 0; row < pf_h; ++row) {
		string new_row = "          ";
		p_pf_ptr->push_back(new_row);
	}

	for (int cell = 0; cell < pf_w; ++cell) {
		// pf_val[8][cell] = BLUE_CELL;
		// pf_val[9][cell] = (cell % 2 == 0 ? EMPTY_CELL : BLUE_CELL);
	}
}

void updateHighScore(int p_score, Text* p_high_score_text) {
	if (save_data.m_SS_map.find(cur_settings_ptr->hash()) == save_data.m_SS_map.end()) {
		save_data.m_SS_map[cur_settings_ptr->hash()] = {p_score, 0, 0, 0, 0};
	}
	else {
		int i = 0;
		while (p_score <= save_data.m_SS_map[cur_settings_ptr->hash()][i]) {
			i++;
		}
		for (int j = i + 1; j < 5; j++) {
			save_data.m_SS_map[cur_settings_ptr->hash()][j] = save_data.m_SS_map[cur_settings_ptr->hash()][j - 1];
		}
		save_data.m_SS_map[cur_settings_ptr->hash()][i] = p_score;
	}

	p_high_score_text->setText(to_string(save_data.m_SS_map[cur_settings_ptr->hash()][0]));
}

void resetGame(Piece* p_cur_piece_ptr, pf_t* p_pf_ptr, vector<int>* p_INP_ptr, vector<int>* p_INP_inv_ptr, array<Text*, 8>& p_texts_ptr, int& p_lines_cleared, int& p_score, int& p_held_piece) {
	updateHighScore(p_score, p_texts_ptr[1]);
	
	ticks_between_falls = gravity[start_level];
	ticks_between_falls_sd = ticks_between_falls / 2;
	tick_counter = 0;
	p_held_piece = -1;

	p_score = 0;
	cur_level = start_level;
	p_lines_cleared = 0;

	p_texts_ptr[0]->setText("0");
	p_texts_ptr[2]->setText("0");
	p_texts_ptr[3]->setText(to_string(cur_level));

	// setFirstLineClearsToAdvance();

	if (cur_settings_ptr->m_piece_gen_mode == piece_gen_modes::Bag) {
		*p_INP_ptr = vector<int>(Piece::new_bag.begin(), Piece::new_bag.end());
		*p_INP_inv_ptr = vector<int>(Piece::new_bag.begin(), Piece::new_bag.end());
		random_shuffle(p_INP_ptr->begin(), p_INP_ptr->end());
		random_shuffle(p_INP_inv_ptr->begin(), p_INP_inv_ptr->end());
	}
	else if (cur_settings_ptr->m_piece_gen_mode == piece_gen_modes::NES || cur_settings_ptr->m_piece_gen_mode == piece_gen_modes::TrueRand){
		*p_INP_ptr = {getRandInt(0, 6)};
	}

	buildPlayField(p_pf_ptr);
	p_cur_piece_ptr->replaceSelf(p_cur_piece_ptr, p_INP_ptr, p_INP_inv_ptr);
}

void updateLevel(int& p_lines_cleared, int p_new_lines_cleared, Text* p_level_text, Text* p_lines_cleared_text) {
	// if (cur_settings_ptr->m_lvl_prog_mode != lvl_prog_modes::NoProgression && p_lines_cleared >= lines_cleared_to_advance) {
		// cur_level++;

		// lines_cleared_to_advance += LCTA_inc;
	// }

	int at = 0; // action type
	int tt = 1; // ...
	int av = 2;
	int tv = 3;
	int r  = 4;

	if (cust_lvl_prog.empty() || cur_settings_ptr->m_lvl_prog_mode == lvl_prog_modes::NoProgression) {
		p_lines_cleared += p_new_lines_cleared;
	}

	else {
		while (p_new_lines_cleared > 0) {
			bool intvl;
			int old_index = -1;
			int old_intvl_rep = 0;

			for (int i = 0; i < cust_lvl_prog.size(); ++i) {
				if (p_lines_cleared >= cust_lvl_prog[i][tv]) {
					old_index = i;
				}
			}

			if ((tholds) cust_lvl_prog[old_index][tt] == tholds::IntF || (tholds) cust_lvl_prog[old_index][tt] == tholds::IntIF) {
				intvl = true;

				while (cust_lvl_prog[old_index][tv] * (old_intvl_rep + 1) < p_lines_cleared) {
					++old_intvl_rep;
				}
			}

			++p_lines_cleared;
			--p_new_lines_cleared;

			int new_index = -1;
			int new_intvl_rep = 0;

			for (int i = 0; i < cust_lvl_prog.size(); ++i) {
				if (p_lines_cleared >= cust_lvl_prog[i][tv]) {
					new_index = i;
				}
			}

			int action_val = cust_lvl_prog[new_index][av];
			
			if (old_index != new_index && !intvl) {
				switch ((actions) cust_lvl_prog[new_index][at]) {
					case actions::Inc:
						cur_level += action_val;
						break;
					case actions::Dec:
						cur_level -= action_val;
						break;
					case actions::Jmp:
						cur_level = action_val;
						break;
				}
			}
			// Didn't cross the threshold for the next event but did for current event (repeating interval)
			else if (intvl) {
				while (cust_lvl_prog[new_index][tv] * (new_intvl_rep + 1) < p_lines_cleared) {
					++new_intvl_rep;
				}
				
				if (new_intvl_rep != old_intvl_rep) {
					cur_level += (actions) cust_lvl_prog[new_index][at] == actions::Inc ? action_val : -action_val;
				}
			}
		}
	}

	ticks_between_falls = gravity[cur_level];
	ticks_between_falls_sd = ticks_between_falls / 2;

	p_level_text->setText(to_string(cur_level));
	p_lines_cleared_text->setText(to_string(p_lines_cleared));
}

vector<int> getFullRows(pf_t* p_pf_ptr) {
	vector<int> full_rows;

	for (int row = FVR; row < pf_h; row++) {
		int empty_cells = 0;

		for (int col = 0; col < pf_w; col++) {
			if ((*p_pf_ptr)[row][col] == EMPTY_CELL) {
				empty_cells++;
			}
		}

		if (empty_cells == 0) {
			full_rows.push_back(row);
		}
	}

	return full_rows;
}

void clearRows(pf_t* p_pf_ptr, vector<int>& p_rows_to_clear) {
	for (int row : p_rows_to_clear) {
		for (int mod_row = row; mod_row >= FVR; mod_row--) {
			(*p_pf_ptr)[mod_row] = (*p_pf_ptr)[mod_row-1];
		}
	}
}

void drawPiecePreviews(vector<int>* p_INP_ptr) {
	for (int i = 0; i < cur_settings_ptr->m_piece_previews; i++) {
		tetromino_t tet = tetrominos[(*p_INP_ptr)[i]];
		for (int row = 0; row < 4; ++row) {
			for (int col = 0; col < 4; col++) {
				if ((char) tet[0][row][col] != EMPTY_CELL) {
					SDL_Rect r;
					r.x = col * grid_cell_w + pp_left; // pf_left/top in pixels
					r.y = row * grid_cell_w + pp_top + 3*grid_cell_w*i;
					r.w = grid_cell_w;
					r.h = grid_cell_w;
					array<Uint8, 3> color = GCCM[tet[0][2][2]];
					
					SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);
					SDL_RenderFillRect(renderer, &r);
				}		
			}
		}
	}
}

void drawHeldPiece(int p_held_piece) {
	tetromino_t tet = tetrominos[p_held_piece];

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; col++) {
			if ((char) tet[0][row][col] != EMPTY_CELL) {
				SDL_Rect r;
				r.x = col * grid_cell_w + hp_left; // pf_left/top in pixels
				r.y = row * grid_cell_w + hp_top;
				r.w = grid_cell_w;
				r.h = grid_cell_w;
				array<Uint8, 3> color = GCCM[tet[0][2][2]];
				
				SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);
				SDL_RenderFillRect(renderer, &r);
			}
		}
	}
}

void updateScore(int p_rows_cleared, Text* score_text, bool& p_prev_clear_was_tetris) {
	switch (cur_settings_ptr->m_scoring_mode) {
		case scoring_modes::NES:
			switch (p_rows_cleared) {
				case 1:
					score += 40 * (cur_level + 1);
					break;
				case 2:
					score += 100 * (cur_level + 1);
					break;
				case 3:
					score += 300 * (cur_level + 1);
					break;
				case 4:
					score += 1200 * (cur_level + 1);
					break;
			}
			break;

		case scoring_modes::Tetris4D:
			switch (p_rows_cleared) {
				case 1:
					score += 80 * (cur_level + 1);
					break;
				case 2:
					score += 200 * (cur_level + 1);
					break;
				case 3:
					score += 600 * (cur_level + 1);
					break;
				case 4:
					score += 2400 * (cur_level + 1);
					break;
			}
			break;

		case scoring_modes::Modern:
			switch (p_rows_cleared) {
				case 1:
					score += 100 * cur_level;
					p_prev_clear_was_tetris = false;
					break;
				case 2:
					score += 300 * cur_level;
					p_prev_clear_was_tetris = false;
					break;
				case 3:
					score += 500 * cur_level;
					p_prev_clear_was_tetris = false;
					break;
				case 4:
					if (p_prev_clear_was_tetris) {
						score += 800 * cur_level * 1.5;
					}
					else {
						score += 800 * cur_level;
						p_prev_clear_was_tetris = true;
					}
					break;
			}
			break;
	}

	score_text->setText(to_string(score));
}

void drawPlayField(pf_t* p_pf_ptr) {
	SDL_Point points[5] = {
		{pf_left  - pf_border_w, pf_top    - pf_border_w},
		{pf_right + pf_border_w, pf_top    - pf_border_w},
		{pf_right + pf_border_w, pf_bottom + pf_border_w},
		{pf_left  - pf_border_w, pf_bottom + pf_border_w},
		{pf_left  - pf_border_w, pf_top    - pf_border_w}
	};

	// SDL_gfx
	// thickLineRGBA(renderer_ptr, points[0].x, points[0].y, points[1].x, points[1].y, 5, 255, 255, 255, 255);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLines(renderer, points, 5);

	for (int row = FVR; row < pf_h; ++row) {
		for (int col = 0; col < pf_w; ++col) {
			char grid_cell = (*p_pf_ptr)[row][col];

			if (grid_cell != EMPTY_CELL) {
				SDL_Rect r;
				r.x = col * grid_cell_w + pf_left;
				r.y = (row - FVR) * grid_cell_w + pf_top;
				r.w = grid_cell_w;
				r.h = grid_cell_w;

				SDL_SetRenderDrawColor(renderer, GCCM[grid_cell][0], GCCM[grid_cell][1], GCCM[grid_cell][2], 255);
				SDL_RenderFillRect(renderer, &r);
			}
		}
	}
}

void lineClearAnimation(SDL_Event& p_event, pf_t* p_pf_ptr, vector<int>& p_full_rows, vector<int>* p_INP_ptr, array<Text*, 8>& p_texts_ptr, int p_held_piece) {
	int delay_counter = 0;
	int columns_from_side = 0;

	while (columns_from_side < pf_w/2 + 1) {
		// _________________________ EVENTS _________________________
		while (SDL_PollEvent(&p_event)) {
			processQuitEvents(p_event);
		}
		//  ________________________ LOGIC __________________________
		if (++delay_counter % 4 == 0) {
			for (int row : p_full_rows) {
				(*p_pf_ptr)[row][columns_from_side] = EMPTY_CELL;
				(*p_pf_ptr)[row][pf_w-1 - columns_from_side] = EMPTY_CELL;
			}
			columns_from_side++;
		}

		//  ________________________ DRAWING ________________________
		SDL_SetRenderDrawColor(renderer, black[0], black[1], black[2], 255);
		SDL_RenderClear(renderer);
		
		drawPlayField(p_pf_ptr);
		
		if (cur_settings_ptr->m_piece_preview_on) {
			drawPiecePreviews(p_INP_ptr);
		}
		if (cur_settings_ptr->m_piece_hold_on && p_held_piece != -1) {
			drawHeldPiece(p_held_piece);
		}
		for (Text* t : p_texts_ptr) {
			t->draw();
		}
	
		SDL_RenderPresent(renderer);
		limitTPS();
	}
}

void gameOverAnimation(SDL_Event& p_event, pf_t* p_pf_ptr, vector<int>* p_INP_ptr, array<Text*, 8>& p_texts_ptr, int p_held_piece) {
	int delay_counter = 0;
	int rows_from_top = pf_h - 1;

	while (rows_from_top >= FVR) {
		// _________________________ EVENTS _________________________
		while (SDL_PollEvent(&p_event)) {
			processQuitEvents(p_event);
		}

		//  ________________________ LOGIC __________________________
		if (++delay_counter % 4 == 0) {
			for (int col = 0; col < pf_w; col++) {
				(*p_pf_ptr)[rows_from_top][col] = WHITE_CELL;
			}
			--rows_from_top;
		}

		//  ________________________ DRAWING ________________________
		SDL_SetRenderDrawColor(renderer, black[0], black[1], black[2], 255);
		SDL_RenderClear(renderer);
		
		drawPlayField(p_pf_ptr);

		if (cur_settings_ptr->m_piece_preview_on) {
			drawPiecePreviews(p_INP_ptr);
		}
		if (cur_settings_ptr->m_piece_hold_on && p_held_piece != -1) {
			drawHeldPiece(p_held_piece);
		}
		for (Text* t : p_texts_ptr) {
			t->draw();
		}
	
		SDL_RenderPresent(renderer);
		limitTPS();
	}
}

void gameLoop() {
	vector<int>* INP_ptr = new vector<int>; // Indexes of next pieces
	vector<int>* INP_inv_ptr = new vector<int>; // Indexes of next pieces inverse (pieces left "in the bag")
	int held_piece = -1;
	vector<kc> LDKP; // Last direction keys pressed
	pf_t pf_val;
	pf_t* pf_ptr = &pf_val;
	Piece* cur_piece_ptr = new Piece();
	bool is_sd = false;
	bool prev_clear_was_tetris = false;
	int lines_cleared = 0;

	SDL_Rect top_cover;
	top_cover.x = pf_left;
	top_cover.y = 0;
	top_cover.w = pf_w * grid_cell_w;
	top_cover.h = pf_top - pf_border_w;

	cur_level = start_level;
	int high_score = save_data.m_SS_map[cur_settings_ptr->hash()][0];

	int x_c = pf_left - grid_cell_w * 4;
	int x_r = pf_left - grid_cell_w;

	int f_size    = grid_cell_w * 0.85;
	int vert_dist = grid_cell_w;

	int y1 = pf_bottom - vert_dist * 12;
	int y2 = pf_bottom - vert_dist * 11;
	int y3 = pf_bottom - vert_dist * 9;
	int y4 = pf_bottom - vert_dist * 8;
	int y5 = pf_bottom - vert_dist * 6;
	int y6 = pf_bottom - vert_dist * 5;
	int y7 = pf_bottom - vert_dist * 3;
	int y8 = pf_bottom - vert_dist * 2;

	array<Text*, 8> texts_ptr = {
		new Text(x_c, y2, "0",                      f_size, &white, -1, jst::C),
		new Text(x_c, y4, to_string(high_score),    f_size, &white, -1, jst::C),
		new Text(x_c, y6, to_string(lines_cleared), f_size, &white, -1, jst::C),
		new Text(x_c, y8, to_string(cur_level),     f_size, &white, -1, jst::C),

		new Text(x_c, y1, "Score",      f_size, &white, -1, jst::C),
		new Text(x_c, y3, "High Score", f_size, &white, -1, jst::C),
		new Text(x_c, y5, "Lines",      f_size, &white, -1, jst::C),
		new Text(x_c, y7, "Level",      f_size, &white, -1, jst::C)
	};
	
	resetGame(cur_piece_ptr, pf_ptr, INP_ptr, INP_inv_ptr, texts_ptr, lines_cleared, score, held_piece);
	
	SDL_Event event;
	while (cur_screen_func_ptr = &gameLoop) {
		// _________________________ EVENTS _________________________
		{
			while (SDL_PollEvent(&event)) {
				processQuitEvents(event);
				kc key;

				if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && !event.key.repeat) {
					key = (kc) event.key.keysym.sym;
				}

				if (event.type == SDL_KEYDOWN && !event.key.repeat) {
					if (key == kc::Space) {
						cur_screen_func_ptr = &pauseLoop;
						pauseLoop();

						break;
					}
					cur_piece_ptr->handleKeyDown(cur_piece_ptr, pf_ptr, key, score, is_sd);

					if (isDirectionKey(key)) {
						LDKP.push_back(key);
					}

					else if (key == kc::Reset) {
						resetGame(cur_piece_ptr, pf_ptr, INP_ptr, INP_inv_ptr, texts_ptr, lines_cleared, score, held_piece);
					}

					else if (key == kc::Enter) {
						cur_settings_ptr->m_piece_preview_on = !cur_settings_ptr->m_piece_preview_on;
					}

					else if (key == kc::Hold && cur_settings_ptr->m_piece_hold_on) {
						if (held_piece != -1) {
							int temp = cur_piece_ptr->getTetrominoIndex();
							*cur_piece_ptr = Piece(pf_w/2 - 2, FVR-2 + cur_settings_ptr->m_piece_start_height, 0, held_piece, cur_piece_ptr->getLongDelayInProgress(), cur_piece_ptr->getShortDelayInProgress());
							held_piece = temp;
						}
						else {
							held_piece = cur_piece_ptr->getTetrominoIndex();
							cur_piece_ptr->replaceSelf(cur_piece_ptr, INP_ptr, INP_inv_ptr);
						}
					}
				}
				
				else if (event.type == SDL_KEYUP && !event.key.repeat) {
					cur_piece_ptr->handleKeyUp(cur_piece_ptr, pf_ptr, key);

					if (isDirectionKey(key)) {
						auto to_remove = find(LDKP.begin(), LDKP.end(), key);
						if (to_remove != LDKP.end()) {
							LDKP.erase(to_remove);
						}
						cur_piece_ptr->handleKeyDown(cur_piece_ptr, pf_ptr, (kc) LDKP[LDKP.size()-1], score, is_sd);
					}
				}
			}
		}
		if (cur_screen_func_ptr != &gameLoop) {
			break;
		}
		//  ________________________ LOGIC __________________________
		{
			is_sd = key_states[(int)sc::Down] && (
				(cur_settings_ptr->m_movement_mode == movement_modes::AllowMultipleKeyHolds) ||
				(cur_settings_ptr->m_movement_mode == movement_modes::OneKeyHeldAtATime && 
				!key_states[(int)sc::Left] && !key_states[(int)sc::Right]) 
			);

			if (!is_sd && ++tick_counter >= ticks_between_falls) {
				cur_piece_ptr->fallOneBlock(cur_piece_ptr, pf_ptr);
				texts_ptr[0]->setText(to_string(score));
				tick_counter = 0;
			}
			else if (is_sd && ++tick_counter >= ticks_between_falls_sd) {
				cur_piece_ptr->fallOneBlock(cur_piece_ptr, pf_ptr);
				texts_ptr[0]->setText(to_string(score));
				tick_counter = 0;
				++score;
			}

			if (cur_piece_ptr->areInProgress()) {
				cur_piece_ptr->areWait((pf_h - cur_piece_ptr->getLowestRow()) / 4 + 10);

				if (!cur_piece_ptr->areInProgress()) {
					cur_piece_ptr->addToPF(pf_ptr);
					vector<int> full_rows = getFullRows(pf_ptr);

					if (full_rows.size() > 0) {
						lineClearAnimation(event, pf_ptr, full_rows, INP_ptr, texts_ptr, held_piece);
						clearRows(pf_ptr, full_rows);
						updateScore(full_rows.size(), texts_ptr[0], prev_clear_was_tetris);
						updateLevel(lines_cleared, full_rows.size(), texts_ptr[3], texts_ptr[2]);
					}
					
					cur_piece_ptr->replaceSelf(cur_piece_ptr, INP_ptr, INP_inv_ptr);
					// Game over
					if (cur_piece_ptr->isColliding(pf_ptr)) {
						gameOverAnimation(event, pf_ptr, INP_ptr, texts_ptr, held_piece);
						resetGame(cur_piece_ptr, pf_ptr, INP_ptr, INP_inv_ptr, texts_ptr, lines_cleared, score, held_piece);
						cur_screen_func_ptr = &mainMenuLoop;
						break;
					}
				}
			}
			else {
				cur_piece_ptr->handleKeyHold(cur_piece_ptr, pf_ptr, LDKP);
			}
		}
		// ________________________ DRAWING _________________________
		{
			SDL_SetRenderDrawColor(renderer, black[0], black[1], black[2], 255);
			SDL_RenderClear(renderer);

			if (cur_settings_ptr->m_piece_preview_on) {
				drawPiecePreviews(INP_ptr);
			}
			if (cur_settings_ptr->m_piece_hold_on && held_piece != -1) {
				drawHeldPiece(held_piece);
			}
			cur_piece_ptr->draw();
			drawPlayField(pf_ptr);

			for (Text* t : texts_ptr) {
				t->draw();
			}
			
			SDL_SetRenderDrawColor(renderer, black[0], black[1], black[2], 255);
			SDL_RenderFillRect(renderer, &top_cover);
			
			SDL_RenderPresent(renderer);
			limitTPS();
		}
	}
}
