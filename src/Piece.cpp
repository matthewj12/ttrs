#include "Piece.h"

vector<int> Piece::new_bag = {0, 1, 2, 3, 4, 5, 6};

Piece::Piece(){
	m_tetromino_index = -1;
	m_long_delay_in_progress = false;
	m_short_delay_in_progress = false;
}

Piece::Piece(int p_x, int p_y, int p_rotation_index, int p_tetromino_index, bool p_long_delay_in_progress, bool p_short_delay_in_progress) {
	m_x = p_x;
	m_y = p_y;
	m_rotation_index = p_rotation_index;
	m_tetromino_index = p_tetromino_index;
	m_tetromino = tetrominos[p_tetromino_index];
	m_long_delay_in_progress = p_long_delay_in_progress;
	m_short_delay_in_progress = p_short_delay_in_progress;
	m_rl_delay_counter = 0; // Right left
	m_d_delay_counter = 0; // drop
	m_are_in_progress = false;
	m_color = GCCM[m_tetromino[p_rotation_index][2][2]]; // Guaranteed to have a block
}

void Piece::updateTetType(int p_new_tet_index) {
	m_tetromino_index = p_new_tet_index;

	m_color = GCCM[m_tetromino[m_rotation_index][2][2]];
}

bool Piece::areInProgress() { return m_are_in_progress; }

void Piece::addToPF(pf_t* p_pf_ptr) {
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; col++) {
			if (m_tetromino[m_rotation_index][row][col] != EMPTY_CELL) {
				(*p_pf_ptr)[m_y + row][m_x + col] = m_tetromino[m_rotation_index][row][col];
			}
		}
	}
}

void Piece::rotate(bool p_direction_is_clockwise, pf_t* p_pf_ptr) {
	m_rotation_index += (p_direction_is_clockwise ? 1 : -1);

	if (m_rotation_index < 0) {
		m_rotation_index = m_tetromino.size() - 1;
	} 
	else if (m_rotation_index == m_tetromino.size()) {
		m_rotation_index = 0;
	}

	if (isColliding(p_pf_ptr)) {
		if (cur_settings_ptr->m_wall_kick_on) {
			for (int i = 0; i < 2 && isColliding(p_pf_ptr); i++) {
				m_x++;
			}
			for (int i = 0; i < 4 && isColliding(p_pf_ptr); i++) {
				m_x--;
			}
			if (isColliding(p_pf_ptr)) { // Collided on bottom
				m_x += 2;
				rotate(!p_direction_is_clockwise, p_pf_ptr);
			}
		}
		else {
			rotate(!p_direction_is_clockwise, p_pf_ptr);
		}

	}
}

void Piece::moveOneBlock(bool p_direction_is_right, pf_t* p_pf_ptr) {
	m_x += (p_direction_is_right ? 1 : -1);

	if (isColliding(p_pf_ptr)) {
		m_x -= (p_direction_is_right ? 1 : -1);
	}
}

void Piece::draw() {
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; col++) {
			if ((char) m_tetromino[m_rotation_index][row][col] != EMPTY_CELL) {
				SDL_Rect r;
				r.x = (m_x + col) * grid_cell_w + pf_left; // pf_left/top in pixels
				r.y = (m_y + row - FVR) * grid_cell_w + pf_top;
				r.w = grid_cell_w;
				r.h = grid_cell_w;
				
				SDL_SetRenderDrawColor(renderer, m_color[0], m_color[1], m_color[2], 255);
				SDL_RenderFillRect(renderer, &r);
			}		
		}
	}
}

bool Piece::isColliding(pf_t* p_pf_ptr) {
	for (int row = 0; row < m_tetromino[0].size(); row++) {
		for (int col = 0; col < m_tetromino[0][0].size(); col++) {
			if ((char) m_tetromino[m_rotation_index][row][col] != EMPTY_CELL) {
				if (m_x + col < 0 || m_y + row < 0 || m_x + col >= pf_w || m_y + row >= pf_h) {
					return true;
				}
				if ((char) (*p_pf_ptr)[m_y + row][m_x + col] != EMPTY_CELL) {
					return true;
				}
			}
		}
	}

	return false;
}

int Piece::getTetrominoIndex() { return m_tetromino_index; }

void Piece::resetDelays() {
	m_long_delay_in_progress = false;
	m_short_delay_in_progress = false;
	m_rl_delay_counter = 0;
}

void Piece::handleKeyDown(Piece* p_cur_piece_ptr, pf_t* p_pf_ptr, kc p_key, int& p_score, bool p_is_sd) {
	if (!m_are_in_progress) {
		switch (p_key) {
			case kc::A:
				rotate(false, p_pf_ptr);
				break;
			case kc::B:
				rotate(true, p_pf_ptr);
				break;
			case kc::Left:
				if ((cur_settings_ptr->m_movement_mode == movement_modes::OneKeyHeldAtATime && !key_states[(int)sc::Right]) || cur_settings_ptr->m_movement_mode == movement_modes::AllowMultipleKeyHolds) {
					resetDelays();
					m_long_delay_in_progress = true;
					moveOneBlock(false, p_pf_ptr);
					break;
				}
			case kc::Right:
				if ((cur_settings_ptr->m_movement_mode == movement_modes::OneKeyHeldAtATime && !key_states[(int)sc::Left]) || cur_settings_ptr->m_movement_mode == movement_modes::AllowMultipleKeyHolds) {
					resetDelays();
					m_long_delay_in_progress = true;
					moveOneBlock(true, p_pf_ptr);
					break;
				}
			case kc::Up:
					break;
			case kc::Down:
				resetDelays();
				m_short_delay_in_progress = true;
				fallOneBlock(p_cur_piece_ptr, p_pf_ptr);
				++p_score;
				break;
		}
	}
	else {
		switch (p_key) {
			case kc::Left:
				if ((cur_settings_ptr->m_movement_mode == movement_modes::OneKeyHeldAtATime && !key_states[(int)sc::Right]) || cur_settings_ptr->m_movement_mode == movement_modes::AllowMultipleKeyHolds) {
					m_long_delay_in_progress = false;
					m_short_delay_in_progress = true;
					break;
				}
			case kc::Right:
				if ((cur_settings_ptr->m_movement_mode == movement_modes::OneKeyHeldAtATime && !key_states[(int)sc::Left]) || cur_settings_ptr->m_movement_mode == movement_modes::AllowMultipleKeyHolds) {
					m_long_delay_in_progress = false;
					m_short_delay_in_progress = true;
					break;
				}
			case kc::Down:
				m_long_delay_in_progress = false;
				m_short_delay_in_progress = true;
				break;
		}
	}
}

void Piece::handleKeyUp(Piece* p_cur_piece_ptr, pf_t* p_pf_ptr, kc p_key) {
	if (!m_are_in_progress) {
		switch (p_key) {
			case kc::A:
				break;
			case kc::B:
				break;
			case kc::Left:
				resetDelays();
				break;
			case kc::Right:
				resetDelays();
				break;
			case kc::Up:
				break;
			case kc::Down:
				resetDelays();
				break;
		}
	}
}

void Piece::handleKeyHold(Piece* p_cur_piece_ptr, pf_t* p_pf_ptr, vector<kc>& p_LDKP) {
	if (!m_are_in_progress) {
		// LDKP = last direction key pressed
		bool should_move = false;

		if (m_long_delay_in_progress && ++m_rl_delay_counter >= DAS_long) {
			m_short_delay_in_progress = true;
			m_long_delay_in_progress = false;
			m_rl_delay_counter = 0;
			should_move = true;
		}
		else if (m_short_delay_in_progress && ++m_rl_delay_counter >= DAS_short) {
			m_rl_delay_counter = 0;
			should_move = true;
		}

		if (should_move) {
			if ((cur_settings_ptr->m_movement_mode == movement_modes::OneKeyHeldAtATime && key_states[(int)sc::Left] && !key_states[(int)sc::Right]) || 
				(cur_settings_ptr->m_movement_mode == movement_modes::AllowMultipleKeyHolds && p_LDKP[p_LDKP.size()-1] == kc::Left)) {
				moveOneBlock(false, p_pf_ptr);
			} 
			else if ((cur_settings_ptr->m_movement_mode == movement_modes::OneKeyHeldAtATime && key_states[(int)sc::Right] && !key_states[(int)sc::Left]) ||
					(cur_settings_ptr->m_movement_mode == movement_modes::AllowMultipleKeyHolds && p_LDKP[p_LDKP.size()-1] == kc::Right)) {
				moveOneBlock(true, p_pf_ptr);
			}
		}
	}
}

void Piece::fallOneBlock(Piece* p_cur_piece_ptr, pf_t* p_pf_ptr) {
	m_y++;

	if (isColliding(p_pf_ptr)) {
		m_y--;
		m_are_in_progress = true;
	}
}

void Piece::areWait(int p_delay) {
	static int delay_counter = 0;

	if (delay_counter++ == p_delay) {
		m_are_in_progress = false;
		delay_counter = 0;
	}
}

int Piece::getLowestRow() {
	int max_row = 0;

	for (int row = 0; row < 4; ++row) {
		if (m_tetromino[m_rotation_index][row] != "    ") {
			if (m_y + row > max_row) {
				max_row = m_y + row;
			}
		}
	}

	return max_row;
}

void Piece::replaceSelf(Piece* p_cur_piece_ptr, vector<int>* p_INP_ptr, vector<int>* p_INP_inv_ptr) {
	int new_index;

	switch (cur_settings_ptr->m_piece_gen_mode) {
		case piece_gen_modes::TrueRand: {
			p_INP_ptr->push_back(getRandInt(0, 6));
			break;
		}
		case piece_gen_modes::NES: {
			new_index = 2;
			// int new_index = getRandInt(0, 6);
			// if (p_cur_piece_ptr->getTetrominoIndex() == new_index) {
			// 	new_index = getRandInt(0, 6);
			// }
			p_INP_ptr->push_back(new_index);
			break;
		}	
		case piece_gen_modes::Bag: {
			if (p_INP_inv_ptr->empty()) {
				*p_INP_inv_ptr = vector<int>(Piece::new_bag.begin(), Piece::new_bag.end());
				random_shuffle(p_INP_inv_ptr->begin(), p_INP_inv_ptr->end());
			}

			p_INP_ptr->push_back((*p_INP_inv_ptr)[0]);
			p_INP_inv_ptr->erase(p_INP_inv_ptr->begin());
			break;
		}
		case piece_gen_modes::Custom: {
			if (p_INP_inv_ptr->empty()) {
				for (array<int, 7> probs : custom_piece_seq) {
					int seed = getRandInt(0, 100);
					float index = 0;

					for (int j = 0; j < 7; j++) {
						index += probs[j];

						if (seed <= index) {
							p_INP_inv_ptr->push_back(j);
							break;
						}
					}
				}
			}

			p_INP_ptr->push_back((*p_INP_inv_ptr)[0]);
			p_INP_inv_ptr->erase(p_INP_inv_ptr->begin());
			break;
		}
	}
	
	*p_cur_piece_ptr = Piece(pf_w/2 - 2, FVR-2 + cur_settings_ptr->m_piece_start_height, 0, (*p_INP_ptr)[0], m_long_delay_in_progress, m_short_delay_in_progress);
	p_INP_ptr->erase(p_INP_ptr->begin());
}
