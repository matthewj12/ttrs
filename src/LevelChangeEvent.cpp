#include "LevelChangeEvent.h"

// Action to string map
unordered_map<actions, string> ASM = {{
	{actions::Inc, "Increase Level"},
	{actions::Dec, "Decrease Level"},
	{actions::Jmp, "Jump to Level"}
}};

// Threshold to string map
unordered_map<tholds, string> TSM = {{
	{tholds::Abs,   "Absolute"},
	{tholds::IntF,  "Interval: Finite"},
	{tholds::IntIF, "Interval: Infinite"}
}};

LevelChangeEvent::LevelChangeEvent(int p_x, int p_y, int p_w, int p_h) {
	m_rect.x = p_x;
	m_rect.y = p_y;
	m_rect.w = p_w;
	m_rect.h = p_h;
	m_action_type = actions::Inc;
	m_thold_type = tholds::Abs;
	m_action_val = 1;
	m_threshold_val = 10;
	m_repetitions = 0;

	int x1 = m_rect.x + m_rect.w * 0.32;
	int x2 = m_rect.x + m_rect.w * 0.4;
	int x3 = m_rect.x + m_rect.w - grid_cell_w / 2;
	int x4 = m_rect.x + m_rect.w + grid_cell_w;

	int y1 = m_rect.y + 0.22 * m_rect.h;
	int y2 = m_rect.y + 0.50 * m_rect.h;
	int y3 = m_rect.y + 0.78 * m_rect.h;

	int font_size = (2.0 / 3.0) * grid_cell_w;
	static string font = "res/fonts/Alien-Encounters-Solid-Regular.ttf";

	m_option_name_texts = {
		new Text(x2, y1, "Type",         font_size, &green, -1, jst::L, jst::C, font),
		new Text(x3, y1, "Value",        font_size, &green, -1, jst::R, jst::C, font),
		new Text(x1, y3, "Action:",      font_size, &green, -1, jst::R, jst::C, font),
		new Text(x1, y2, "Threshold:",   font_size, &green, -1, jst::R, jst::C, font),
		new Text(x4, y1, "Repetitions:", font_size, &green, -1, jst::L, jst::C, font)
	};

	m_option_texts = {
		new Text(x2, y2, TSM[m_thold_type],      font_size, &white,    -1, jst::L, jst::C, font),
		new Text(x3, y2, to_string(m_threshold_val), font_size, &med_grey, -1, jst::R, jst::C, font),
		new Text(x2, y3, ASM[m_action_type],         font_size, &med_grey, -1, jst::L, jst::C, font),
		new Text(x3, y3, to_string(m_action_val),    font_size, &med_grey, -1, jst::R, jst::C, font),
		new Text(x4, y3, "0",                        font_size, &drk_red,  -1, jst::L, jst::C, font)
	};
}

void LevelChangeEvent::draw(int p_selected_pos, int p_this_pos, bool p_editing_event) {
	drawBox(m_rect, 3, p_selected_pos == p_this_pos ? (p_editing_event ? &blue : &white) : &drk_grey);

	for (Text* t : m_option_name_texts) {
		t->draw();
	}
	for (Text* t : m_option_texts) {
		t->draw();
	}
}

void LevelChangeEvent::editProperty(int p_property, bool p_increasing) {
	switch (p_property) {
		case 0:
			if (p_increasing) {
				m_thold_type = (tholds) (((int) m_thold_type + 1) % (int) tholds::Size);
			}
			else {
				m_thold_type = (tholds) ((int) m_thold_type == 0 ? (int) tholds::Size - 1 : (int) m_thold_type - 1);
			}

			m_option_texts[4]->setText(m_thold_type == tholds::IntIF ? "Inf." : (m_thold_type == tholds::Abs ? "0" : to_string(m_repetitions)));
			m_option_texts[4]->setColor(m_thold_type == tholds::IntF ? &med_grey : &drk_red);

			break;

		case 1:
			m_threshold_val += p_increasing ? 1 : -1;

			if (m_threshold_val < 0) {
				m_threshold_val = 9999;
			}
			else if (m_threshold_val > 9999) {
				m_threshold_val = 0;
			}
			break;
		
		case 2:
			if (p_increasing) {
				m_action_type = (actions) (((int) m_action_type + 1) % (int) actions::Size);
			}
			else {
				m_action_type = (actions) ((int) m_action_type == 0 ? (int) actions::Size - 1 : (int) m_action_type - 1);
			}
			break;

		case 3:
			m_action_val += p_increasing ? 1 : -1;

			if (m_action_val < 0) {
				m_action_val = max_selectable_level;
			}
			else if (m_action_val > max_selectable_level) {
				m_action_val = 0;
			}
			break;

		case 4:
			if (m_thold_type == tholds::IntF) {
				m_repetitions += p_increasing ? 1 : -1;

				if (m_repetitions < 0) {
					m_repetitions = 9999;
				}
				else if (m_repetitions > 9999) {
					m_repetitions = 0;
				}
			}
			
			break;
	}

	string new_text;
	if      (p_property == 0) new_text = TSM[m_thold_type];
	else if (p_property == 1) new_text = to_string(m_threshold_val);
	else if (p_property == 2) new_text = ASM[m_action_type];
	else if (p_property == 3) new_text = to_string(m_action_val);
	else                      new_text = m_thold_type == tholds::IntF ? to_string(m_repetitions) : m_option_texts[p_property]->getText();

	m_option_texts[p_property]->setText(new_text);
}

void LevelChangeEvent::shiftY(int p_offset) {
	m_rect.y += p_offset;

	for (Text* t : m_option_texts) {
		t->shiftY(p_offset);
	}
	for (Text* t : m_option_name_texts) {
		t->shiftY(p_offset);
	}
}
