#include "GlobalFunctions.h"

bool isDirectionKey(kc p_key) {
	return p_key == kc::Left || p_key == kc::Right || p_key == kc::Down;
}

int getRandInt(int p_min, int p_max) {
    static constexpr double fraction { 1.0 / (RAND_MAX + 1.0) };
    return p_min + static_cast<int>((p_max - p_min + 1) * (rand() * fraction));
}

void processQuitEvents(SDL_Event& p_event) {
	if (p_event.type == SDL_KEYUP && p_event.key.keysym.sym == SDLK_ESCAPE || p_event.type == SDL_QUIT) {
		program_running = false;
		cur_screen_func_ptr = nullptr;
	}
}

void limitTPS() {
	static Uint32 prev_time;
	static Uint32 cur_time = SDL_GetTicks();
	static int delta_time;

	prev_time = cur_time;
	cur_time = SDL_GetTicks();
	delta_time = cur_time - prev_time;

	if (delta_time < min_time_step) {
		SDL_Delay(min_time_step - delta_time);
	}
}

void drawBox(SDL_Rect p_rect, int p_width, array<Uint8, 3>* p_color_ptr) {
	SDL_SetRenderDrawColor(renderer, (*p_color_ptr)[0], (*p_color_ptr)[1], (*p_color_ptr)[2], 255);

	SDL_RenderFillRect(renderer, &p_rect);

	p_rect.x += p_width;
	p_rect.y += p_width;
	p_rect.w -= p_width * 2;
	p_rect.h -= p_width * 2;

	SDL_SetRenderDrawColor(renderer, black[0], black[1], black[2], 255);
	SDL_RenderFillRect(renderer, &p_rect);
}

void drawBox(int p_x, int p_y, int p_w, int p_h, int p_width, array<Uint8, 3>* p_color_ptr) {
	SDL_Rect r;
	r.x = p_x;
	r.y = p_y;
	r.w = p_w;
	r.h = p_h;

	drawBox(r, p_width, p_color_ptr);
}

void setFirstLineClearsToAdvance() {
	switch (cur_settings_ptr->m_lvl_prog_mode) {
		case lvl_prog_modes::NES:
			lines_cleared_to_advance = min(cur_level * 10 + 10, max(100, cur_level * 10 - 50));
			break;
		case lvl_prog_modes::EveryTenLines:
			lines_cleared_to_advance = 10;
			break;
	}

	LCTA_inc = 10;
}

void loadData() {
	ifstream file("C:/Users/matth/Documents/Coding/C++/SDLTetris/res/data.dat", ios::out | ios::binary);

	if (!file) {
		cout << "Cannot open file!\n";
	}

	while (!file.eof()) {
		int key;
		array<int, 5> val;

		file.read((char*) &key, sizeof(int));
		file.read((char*) &val, sizeof(array<int, 5>));

		save_data.m_SS_map[key] = val;
	}

	file.close();
}

void saveData() {
	ofstream file("C:/Users/matth/Documents/Coding/C++/SDLTetris/res/data.dat", ios::out | ios::binary);

	if (!file) {
		cout << "Cannot open file!\n";
	}

	for (auto it = save_data.m_SS_map.begin(); it != save_data.m_SS_map.end(); it++) {
		file.write((char*) &(it->first), sizeof(int));
		file.write((char*) &(it->second), sizeof(array<int, 5>));
	}

	file.close();

	if (!file.good()) {
		cout << "Error occured at writing time!\n";
	}
}