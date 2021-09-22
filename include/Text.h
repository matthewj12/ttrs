#pragma once

#include "GlobalVariables.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>
#include <iostream>

using namespace std;

enum class jst {L, R, C, T, B};

class Text {
	SDL_Rect m_rect;
	SDL_Texture* m_texture_ptr; 
	int m_size;
	int m_max_w;
	jst m_horz_justification;
	jst m_vert_justification;
	TTF_Font* m_font;
	string m_text;
	SDL_Color m_color;

public:
	Text(int p_x, int p_y, string p_text, int p_size, array<Uint8, 3>* p_color_ptr=new array<Uint8, 3>{255,255,255}, int p_max_w=-1, jst p_horz_justification=jst::C, jst p_vert_justification=jst::T, string p_font_path="res/fonts/seguisb.ttf");
	~Text();
	void draw();
	void setText(string p_text);
	void setColor(array<Uint8, 3>* p_color);
	void setCenter(int p_x, int p_y);
	string getText() { return m_text; }
	string getColor() { return to_string(m_color.r) + ", " + to_string(m_color.g) + ", " + to_string(m_color.b); }
	int getX() { return m_rect.x; }
	int getY() { return m_rect.y; }
	int getW() { return m_rect.w; }
	int getH() { return m_rect.h; }
	int getCenterY() { return m_rect.x + m_rect.w / 2; }
	int getCenterX() { return m_rect.y + m_rect.h / 2; }
	void setLeft(int p_x) { m_rect.x = p_x; }
	void setRight(int p_x) { m_rect.x = p_x - m_rect.w; }
	void shiftX(int p_offset);
	void shiftY(int p_offset) { m_rect.y += p_offset; }
	void setTop(int p_y) { m_rect.y = p_y; }
	void setBottom(int p_y) { m_rect.y = p_y - m_rect.h; }
	void setCenterX(int p_x) { m_rect.x = p_x - m_rect.w / 2; }
	void setCenterY(int p_y) { m_rect.y = p_y - m_rect.h / 2; }
};
