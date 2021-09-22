#include "GlobalVariables.h"
#include "GlobalFunctions.h"
#include "Piece.h"
#include "Tetrominos.h"
#include "Text.h"
#include "LevelChangeEvent.h"

#include <SDL_image.h>
#include <SDL_ttf.h>
// #include <SDL2_gfxPrimitives.h>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

void gameLoop();
void settingsLoop();

void customLevelProgLoop() {
	int pos = 0;
	bool editing_event = false;
	int property_editing = 0;

	int min_y = h_2_top + grid_cell_w * 3;
	int max_y = h_5_top - grid_cell_w * 2;

	SDL_Rect cover_up_top_rect;
	SDL_Rect cover_up_bottom_rect;
	cover_up_top_rect.x = 0;
	cover_up_top_rect.y = 0;
	cover_up_top_rect.w = window_w;
	cover_up_top_rect.h = min_y;
	cover_up_bottom_rect.x = 0;
	cover_up_bottom_rect.y = max_y;
	cover_up_bottom_rect.w = window_w;
	cover_up_bottom_rect.h = window_h - min_y;

	SDL_Rect new_event_rect;
	new_event_rect.w = grid_cell_w * 16;
	new_event_rect.h = grid_cell_w * 4;

	int events_left = window_w / 2 - new_event_rect.w / 2;
	int events_top = min_y;

	// cust_lvl_prog.push_back({
	// 	(int) e->m_action_type, 
	// 	(int) e->m_thold_type, 
	// 	e->m_action_val, 
	// 	e->m_threshold_val, 
	// 	e->m_repetitions
	// });

	vector<LevelChangeEvent*> events;

	for (int i = 0; i < cust_lvl_prog.size(); ++i) {
		events.push_back(new LevelChangeEvent(
			events_left, 
			events.size() == 0 ? events_top : events[i-1]->m_rect.y + events[i-1]->m_rect.h, 
			new_event_rect.w, 
			new_event_rect.h
		));

		events[i]->m_action_type   = (actions) cust_lvl_prog[i][0];
		events[i]->m_thold_type    = (tholds) cust_lvl_prog[i][1];
		events[i]->m_action_val    = cust_lvl_prog[i][2];
		events[i]->m_threshold_val = cust_lvl_prog[i][3];
		events[i]->m_repetitions   = cust_lvl_prog[i][4];

		events[i]->m_option_texts[0]->setText(TSM[events[i]->m_thold_type]);
		events[i]->m_option_texts[1]->setText(to_string(events[i]->m_threshold_val));
		events[i]->m_option_texts[2]->setText(ASM[events[i]->m_action_type]);
		events[i]->m_option_texts[3]->setText(to_string(events[i]->m_action_val));
		events[i]->m_option_texts[4]->setText(events[i]->m_thold_type == tholds::IntF ? to_string(events[i]->m_repetitions) : (events[i]->m_thold_type == tholds::Abs ? "0" : "Inf"));

		events[i]->m_option_texts[4]->setColor(events[i]->m_thold_type == tholds::IntF ? &med_grey : &drk_red);
	}

	new_event_rect.x = events_left;
	new_event_rect.y = events.size() == 0 ? events_top : events[events.size()-1]->m_rect.y + events[events.size()-1]->m_rect.h;

	static Text title_text(0, h_1_top, "Custom Level Progression", lg_font_size);
	static Text play_text(0, h_2_top, "Press Enter to Play", med_font_size, &med_grey);
	static Text return_text(0, h_5_top, "Press Shift to Go Back", med_font_size, &med_grey);

	title_text.setCenterX(window_w / 2);
	play_text.setCenterX(window_w / 2);
	return_text.setCenterX(window_w / 2);

	SDL_Event event;
	SDL_FlushEvent(SDL_KEYDOWN);
	while (cur_screen_func_ptr == &customLevelProgLoop) {
		// _________________________ EVENTS _________________________
		{
			while (SDL_PollEvent(&event)) {
				processQuitEvents(event);

				if (event.type == SDL_KEYDOWN && !event.key.repeat) {
					kc key = (kc) event.key.keysym.sym;

					if (key == kc::Left || key == kc::Right) {
						events[pos]->m_option_texts[property_editing]->setColor(&med_grey);
						
						if (editing_event) {
							if (key == kc::Left) {
								--property_editing;
								if (property_editing < 0) {
									property_editing = 4;
								}
							}
							else {
								++property_editing;
								if (property_editing > 4) {
									property_editing = 0;
								}
							}
						}

						events[pos]->m_option_texts[property_editing]->setColor(&white);
					}

					else if (key == kc::Up || key == kc::Down) {
						if (editing_event) {
							if (key == kc::Down) {
								events[pos]->editProperty(property_editing, false);
							}
							else {
								events[pos]->editProperty(property_editing, true);
							}
						}
						else {
							if (key == kc::Down) {
								pos = (pos + 1) % (events.size() + 1);
							}
							else {
								pos += (pos == 0) ? events.size() : -1;
							}

							if (!events.empty()) {
								int rect_top, rect_bottom;
								if (pos == events.size()) {
									rect_top = new_event_rect.y;
									rect_bottom = new_event_rect.y + new_event_rect.h;
								}
								else {
									rect_top = events[pos]->m_rect.y;
									rect_bottom = events[pos]->m_rect.y + events[pos]->m_rect.h;
								}

								int to_move = max(0, min_y - rect_top);
								if (to_move == 0) to_move = -max(0, rect_bottom - max_y);

								for (LevelChangeEvent* e : events) {
									e->shiftY(to_move);
								}
								new_event_rect.y += to_move;
							}
						}
					}

					else if (key == kc::x) {
						if (!editing_event && pos != events.size()) {
							events.erase(events.begin() + pos);
						}
					}

					else if (key == kc::Space) {
						if (!editing_event) {
							if (pos == events.size()) {
								events.push_back(new LevelChangeEvent(events_left, events.size() == 0 ? events_top : events[events.size()-1]->m_rect.y + events[events.size()-1]->m_rect.h, new_event_rect.w, new_event_rect.h));
								new_event_rect.y += new_event_rect.h;
							}
							else {
								property_editing = 0;
								events[pos]->m_option_texts[property_editing]->setColor(&white);
							}
							editing_event = true;
						}
					}

					else if (key == kc::Enter) {
						cur_screen_func_ptr = &gameLoop;

						for (LevelChangeEvent* e : events) {
							cust_lvl_prog.push_back({
								(int) e->m_action_type, 
								(int) e->m_thold_type, 
								e->m_action_val, 
								e->m_threshold_val, 
								e->m_repetitions
							});
						}

						break;
					}

					else if (key == kc::Shift) {
						if (editing_event) {
							editing_event = false;
							events[pos]->m_option_texts[property_editing]->setColor(&med_grey);
						}
						else {
							cur_screen_func_ptr = &settingsLoop;

							for (LevelChangeEvent* e : events) {
								cust_lvl_prog.push_back({
									(int) e->m_action_type, 
									(int) e->m_thold_type, 
									e->m_action_val, 
									e->m_threshold_val, 
									e->m_repetitions
								});
							}

							break;
						}
					}
				}
			}
		}

		//  ________________________ LOGIC __________________________
		{
			
		}

		// ________________________ DRAWING _________________________
		{
			SDL_SetRenderDrawColor(renderer, black[0], black[1], black[2], 255);
			SDL_RenderClear(renderer);

			int i = 0;
			for (LevelChangeEvent* e : events) {
				e->draw(pos, i, editing_event);
				++i;
			}

			SDL_RenderFillRect(renderer, &cover_up_top_rect);
			SDL_RenderFillRect(renderer, &cover_up_bottom_rect);

			title_text.draw();
			play_text.draw();
			return_text.draw();

			if (pos == events.size()) {
				drawBox(new_event_rect, 3, &white);
			}

			SDL_RenderPresent(renderer);
			limitTPS();
		}
	}
}
