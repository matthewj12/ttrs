#pragma once

#include "GameData.h"
#include "Settings.h"

#include <SDL.h>

#include <array>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
using pf_t = vector<string>;
using tetromino_t = vector<vector<string>>;

enum class actions {
	Inc,
	Dec,
	Jmp,
	Size
};
// thresholds
enum class tholds {
	Abs,
	IntF,
	IntIF,
	Size
};

// Key codes
enum class kc {
	A      = SDLK_z,
	B      = SDLK_x,
	Hold   = SDLK_c,
	Enter  = SDLK_RETURN,
	Reset  = SDLK_r,
	Shift  = SDLK_LSHIFT,
	Ctrl   = SDLK_LCTRL,
	Left   = SDLK_LEFT,
	Right  = SDLK_RIGHT,
	Up     = SDLK_UP,
	Down   = SDLK_DOWN,
	Space  = SDLK_SPACE,
	x      = SDLK_x,
	i      = SDLK_i,
	d      = SDLK_d,
	j      = SDLK_j
};
// Scan codes
enum class sc {
	A      = SDL_SCANCODE_Z,
	B      = SDL_SCANCODE_X,
	Hold   = SDL_SCANCODE_C,
	Enter  = SDL_SCANCODE_RETURN,
	Reset  = SDL_SCANCODE_R,
	Shift  = SDL_SCANCODE_LSHIFT,
	Ctrl   = SDL_SCANCODE_LCTRL,
	Left   = SDL_SCANCODE_LEFT,
	Right  = SDL_SCANCODE_RIGHT,
	Up     = SDL_SCANCODE_UP,
	Down   = SDL_SCANCODE_DOWN,
	Space  = SDL_SCANCODE_SPACE,
	x      = SDL_SCANCODE_X
};

enum class screens {
	None, // Used for program exit
	MainMenu,
	Settings,
	CustomPieceSeq,
	CustomLevelProgression,
	HighScores,
	PlayField,
	Pause,
	Secret
};

extern void (*cur_screen_func_ptr)();

extern Settings* cur_settings_ptr;
extern vector<array<int, 7>> custom_piece_seq;
extern vector<array<int, 5>> cust_lvl_prog;
extern array<array<int, 9>, 2> presets;

extern const char* window_caption;
extern bool program_running;
extern screens cur_screen;
extern string game_path_root;
extern bool piece_previews_changed;

extern const Uint8* key_states;

extern vector<string> levels_0_19;
extern vector<string> levels_1_25;
extern vector<string> piece_previews_1;
extern vector<string> piece_previews_3;
extern vector<string> piece_previews_5;
extern unordered_map<scoring_modes, vector<string>*> selectable_levels;
extern unordered_map<piece_gen_modes, vector<string>*> selectable_piece_previews;

// Level progression mode to Line clears to advance increment map
extern unordered_map<lvl_prog_modes, int> LPM_LCTA_inc;
// {min level, max_level}, frames per gridcell
extern array<int, 40> gravity;
extern int min_level;
extern int max_selectable_level;

// Unit = pixels
extern int window_w, window_h;
extern int grid_cell_w;
// Unit = Grid cells
extern int FVR; // First visible row
extern int pf_border_w;
extern int pf_w, pf_h; // Play field
extern int pf_left, pf_top;
extern int pf_right, pf_bottom;
extern int pp_left, pp_top; // Piece preview
extern int hp_left, hp_top; // Held piece

extern int h_1_top;
extern int h_2_top;
extern int h_3_top;
extern int h_4_top;
extern int h_5_top;
extern int h_6_top;
extern int h_7_top;
extern int button_top;

extern int sm_font_size;
extern int med_font_size;
extern int lg_font_size;

extern int min_time_step;
extern int DAS_long;
extern int DAS_short;

extern int ticks_between_falls;
extern int ticks_between_falls_sd;
extern int tick_counter;

extern int start_level;
extern int cur_level;
extern int lines_cleared_to_advance;
extern int LCTA_inc;
extern int score;
extern GameData save_data;

extern char EMPTY_CELL;
extern char RED_CELL;
extern char GREEN_CELL;
extern char BLUE_CELL;
extern char CYAN_CELL;
extern char MAGENTA_CELL;
extern char YELLOW_CELL;
extern char ORANGE_CELL;
extern char WHITE_CELL;

extern array<Uint8, 3> black;
extern array<Uint8, 3> drk_grey;
extern array<Uint8, 3> med_grey;
extern array<Uint8, 3> lgt_grey;
extern array<Uint8, 3> white;
extern array<Uint8, 3> red;
extern array<Uint8, 3> green;
extern array<Uint8, 3> blue;
extern array<Uint8, 3> cyan;
extern array<Uint8, 3> magenta;
extern array<Uint8, 3> yellow;
extern array<Uint8, 3> orange;
extern array<Uint8, 3> drk_red;

// GDCM = Grid cell to color map
extern unordered_map<char, array<Uint8, 3>> GCCM;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

