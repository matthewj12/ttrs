#include "Text.h"

Text::Text(int p_x, int p_y, string p_text, int p_size, array<Uint8, 3>* p_color_ptr, int p_max_w, jst p_horz_justification, jst p_vert_justification, string p_font_path) {
	m_rect.x = p_x;
	m_rect.y = p_y;
	m_rect.w = 0;
	m_rect.h = 0;
	m_text = p_text;
	m_size = p_size;
	m_color = {(*p_color_ptr)[0], (*p_color_ptr)[1], (*p_color_ptr)[2]};
	m_max_w = p_max_w;
	m_horz_justification = p_horz_justification;
	m_vert_justification = p_vert_justification;
	m_font = TTF_OpenFont((game_path_root + p_font_path).c_str(), m_size);
	if (m_font == NULL) {
        cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError();
		string s;
		cin >> s;
    }
	setText(p_text);

	switch (p_vert_justification) {
		case (jst::T):
			m_rect.y = p_y;
			break;
		case (jst::B):
			m_rect.y = p_y - m_rect.h;
			break;
		case (jst::C):
			m_rect.y = p_y - m_rect.h / 2;
			break;
	}
}

Text::~Text() {
	SDL_DestroyTexture(m_texture_ptr);
	TTF_CloseFont(m_font);
}

void Text::setColor(array<Uint8, 3>* p_color_ptr) {
	m_color = {(*p_color_ptr)[0], (*p_color_ptr)[1], (*p_color_ptr)[2]};
	setText(m_text);
}

void Text::setCenter(int p_x, int p_y) {
	m_rect.x = p_x - m_rect.w / 2;
	m_rect.y = p_y - m_rect.h / 2;
}

void Text::draw() {
	SDL_RenderCopy(renderer, m_texture_ptr, NULL, &m_rect);
}

void Text::setText(string p_text) {
	m_text = p_text;
	int old_w = m_rect.w;

	if (m_max_w == -1) {
		SDL_Surface* temp_surf_ptr = TTF_RenderText_Blended(m_font, m_text.c_str(), m_color);
		m_texture_ptr = SDL_CreateTextureFromSurface(renderer, temp_surf_ptr);
		
		SDL_QueryTexture(m_texture_ptr, NULL, NULL, &m_rect.w, &m_rect.h);
	}
	else {
		int line_height;
		int shrinking_line_w;

		SDL_Surface* temp_surf_ptr = TTF_RenderText_Blended(m_font, m_text.c_str(), m_color);
		SDL_Texture* temp_tex_ptr = SDL_CreateTextureFromSurface(renderer, temp_surf_ptr);

		// line_height will be the height of single line because temp_tex_ptr starts as one line
		SDL_QueryTexture(temp_tex_ptr, NULL, NULL, &shrinking_line_w, &line_height);

		string shrinking_str = m_text;
		string expanding_str = "";
		vector<SDL_Texture*> rendered_lines;
		bool removed_space = false;
		
		while (shrinking_line_w > m_max_w) {
			int pos = shrinking_str.size() - 1;
			if (shrinking_str.find(" ", 0) != string::npos) {
				for (; pos > 0 && shrinking_str[pos] != ' '; pos--){}
			}

			expanding_str.insert(0, shrinking_str.substr(pos, string::npos));
			shrinking_str.erase(pos);

			temp_surf_ptr = TTF_RenderText_Blended(m_font, shrinking_str.c_str(), m_color);
			temp_tex_ptr = SDL_CreateTextureFromSurface(renderer, temp_surf_ptr);
			SDL_QueryTexture(temp_tex_ptr, NULL, NULL, &shrinking_line_w, NULL);

			if (shrinking_line_w <= m_max_w) {
				rendered_lines.push_back(temp_tex_ptr);

				if (expanding_str[0] == ' ') {
					expanding_str.erase(0, 1);
				}
				shrinking_str = expanding_str;
				expanding_str = "";

				temp_surf_ptr = TTF_RenderText_Blended(m_font, shrinking_str.c_str(), m_color);
				temp_tex_ptr = SDL_CreateTextureFromSurface(renderer, temp_surf_ptr);

				SDL_QueryTexture(temp_tex_ptr, NULL, NULL, &shrinking_line_w, NULL);
			}
		}
		// Last line
		temp_surf_ptr = TTF_RenderText_Blended(m_font, shrinking_str.c_str(), m_color);
		temp_tex_ptr = SDL_CreateTextureFromSurface(renderer, temp_surf_ptr);
		SDL_QueryTexture(temp_tex_ptr, NULL, NULL, &shrinking_line_w, NULL);
		rendered_lines.push_back(temp_tex_ptr);

		m_texture_ptr = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_max_w, line_height * rendered_lines.size());
		SDL_SetTextureBlendMode(m_texture_ptr, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0,0,0,0);
		SDL_RenderClear(renderer);

		m_rect.w = m_max_w;
		m_rect.h = line_height * rendered_lines.size();

		SDL_SetRenderTarget(renderer, m_texture_ptr);

		SDL_Rect dst;
		dst.y = 0;
		
		for (SDL_Texture* t : rendered_lines) {
			SDL_QueryTexture(t, NULL, NULL, &dst.w, &dst.h);
			// Renders to m_texture_ptr

			switch (m_horz_justification) {
				case jst::C:
					dst.x = m_max_w / 2 - dst.w / 2;
					break;
				case jst::L:
					dst.x = 0;
					dst.y = 0;
					break;
				case jst::R:
					dst.x = m_max_w - dst.w;
					break;
			}

			SDL_RenderCopy(renderer, t, NULL, &dst);
			dst.y += line_height;
		}

		SDL_SetRenderTarget(renderer, NULL);
	}

	switch (m_horz_justification) {
		case (jst::C):
			m_rect.x -= (m_rect.w - old_w) / 2;
			break;
		case (jst::L):
			break;
		case (jst::R):
			m_rect.x -= (m_rect.w - old_w);
	}
}

void Text::shiftX(int p_offset) {
	m_rect.x += p_offset;
}
