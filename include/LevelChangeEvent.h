#pragma once

#include "GlobalVariables.h"
#include "GlobalFunctions.h"
#include "Text.h"

// Action to string map
extern unordered_map<actions, string> ASM;
// Threshold to string map
extern unordered_map<tholds, string> TSM;

struct LevelChangeEvent {
	actions m_action_type;
	tholds m_thold_type;
	int m_action_val;
	int m_threshold_val;
	int m_repetitions;

	SDL_Rect m_rect;
	
	array<Text*, 5> m_option_name_texts;
	array<Text*, 5> m_option_texts;

	LevelChangeEvent(int p_x, int p_y, int p_w, int p_h);
	void draw(int p_selected_pos, int p_this_pos, bool p_editing_event);
	void editProperty(int p_property, bool p_increasing);
	void shiftY(int p_offset);
};