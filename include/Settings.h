#pragma once

#include <iostream>
#include <unordered_map>

enum class settings_presets {
	NES,
	Modern,
	Custom
};
enum class movement_modes {
	OneKeyHeldAtATime,
	AllowMultipleKeyHolds
};
enum class piece_gen_modes {
	NES,
	Bag,
	TrueRand,
	Custom
};
enum class scoring_modes {
	NES,
	Tetris4D,
	Modern
};

enum class lvl_prog_modes {
	NES,
	EveryTenLines,
	NoProgression,
	Custom
};

struct Settings {
	movement_modes m_movement_mode;
	piece_gen_modes m_piece_gen_mode;
	scoring_modes m_scoring_mode;
	lvl_prog_modes m_lvl_prog_mode;
	int m_piece_start_height;
	bool m_wall_kick_on;
	bool m_piece_hold_on;
	bool m_piece_preview_on;
	int m_piece_previews;

	Settings();	
	Settings(
		int p_movement_mode, 
		int p_piece_gen_mode, 
		int p_scoring_mode, 
		int p_lvl_prog_mode,
		int p_piece_start_height, 
		bool p_wall_kick_on, 
		bool p_piece_hold_on, 
		bool p_piece_preview_on, 
		int p_piece_previews)
	{
		m_movement_mode = (movement_modes) p_movement_mode;
		m_piece_gen_mode = (piece_gen_modes) p_piece_gen_mode;
		m_scoring_mode = (scoring_modes) p_scoring_mode;
		m_lvl_prog_mode = (lvl_prog_modes) p_lvl_prog_mode;
		m_piece_start_height = p_piece_start_height;
		m_wall_kick_on = p_wall_kick_on;
		m_piece_hold_on = p_piece_hold_on;
		m_piece_preview_on = p_piece_preview_on;
		m_piece_previews = p_piece_previews;
	}

	int hash() {
		return ((int) m_scoring_mode) + 
		              m_piece_previews * 10 + 
			   ((int) m_piece_hold_on) * 100 + 
			//    ((int) m_lvl_prog_mode) * 1000 + 
			   ((int) m_piece_gen_mode) * 10000;
	}
};