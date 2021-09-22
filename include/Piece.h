#pragma once

#include "GlobalVariables.h"
#include "GlobalFunctions.h"
#include "Tetrominos.h"

#include <array>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

using tetromino_t = vector<vector<string>>;

class Piece {
	int m_x;
	int m_y;
	int m_rotation_index;
	int m_tetromino_index;
	bool m_long_delay_in_progress;
	bool m_short_delay_in_progress;
	bool m_are_in_progress;
	int m_rl_delay_counter;
	int m_d_delay_counter;
	array<Uint8, 3> m_color;
	tetromino_t m_tetromino;
public:
	static vector<int> new_bag;
	Piece();
	Piece(int p_x, int p_y, int p_rotation_index, int p_tetromino_index, bool p_long_delay_in_progress, bool p_short_delay_in_progress);
	int getX() { return m_x; }
	int getY() { return m_y; }
	void updateTetType(int p_new_tet_index);
	bool areInProgress();
	void addToPF(pf_t* p_pf);
	void rotate(bool p_directionIsClockwise, pf_t* p_pf);
	void moveOneBlock(bool directionIsRight, pf_t* p_pf);
	void draw();
	bool isColliding(pf_t* p_pf);
	int getTetrominoIndex();
	bool getLongDelayInProgress() { return m_long_delay_in_progress; }
	bool getShortDelayInProgress() { return m_short_delay_in_progress; }
	void resetDelays();
	void handleKeyDown(Piece* p_cur_piece_ptr, pf_t* p_pf_ptr, kc p_key, int& p_score, bool p_is_sd);
	void handleKeyUp(Piece* p_cur_piece_ptr, pf_t* p_pf_ptr, kc p_key);
	// For movement from holding right/left key
	void handleKeyHold(Piece* p_cur_piece_ptr, pf_t* p_pf_ptr, vector<kc>& p_LDKP);
	void fallOneBlock(Piece* p_cur_piece_ptr, pf_t* p_pf_ptr);
	void areWait(int p_delay);
	int getLowestRow();
	void replaceSelf(Piece* p_cur_piece_ptr, vector<int>* p_INP_ptr, vector<int>* p_INP_inv_ptr);
};

