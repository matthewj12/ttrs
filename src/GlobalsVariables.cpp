#include "GlobalVariables.h"

void (*cur_screen_func_ptr)();

vector<array<int, 7>> custom_piece_seq = {};
vector<array<int, 5>> cust_lvl_prog;
array<array<int, 9>, 2> presets = {{
	{0,0,0,0,0,0,0,1,1},
	{1,1,2,1,1,1,1,1,3}
}};
array<int, 9> p = presets[0];
Settings* cur_settings_ptr = new Settings(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8]);

const char* window_caption = "TetrisEX";
bool program_running = true;
string game_path_root = "../../";
bool piece_previews_changed = false;

const Uint8* key_states = SDL_GetKeyboardState(nullptr);

vector<string> levels_0_19 = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19"};
vector<string> levels_1_25 = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25"};
vector<string> piece_previews_7 = {"0", "1", "2", "3", "4", "5", "6", "7"};
vector<string> piece_previews_3 = {"0", "1", "2", "3"};
vector<string> piece_previews_1 = {"0", "1"};

unordered_map<scoring_modes, vector<string>*> selectable_levels = {
	{scoring_modes::NES, &levels_0_19},
	{scoring_modes::Tetris4D, &levels_0_19},
	{scoring_modes::Modern, &levels_1_25}
};
unordered_map<piece_gen_modes, vector<string>*> selectable_piece_previews = {
	{piece_gen_modes::NES, &piece_previews_1},
	{piece_gen_modes::TrueRand, &piece_previews_1},
	{piece_gen_modes::Bag, &piece_previews_7},
	{piece_gen_modes::Custom, &piece_previews_7},
};

array<int, 40> gravity = {48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int min_level = 0;
int max_selectable_level = 29;
int start_level = 1;

int h_1_top = grid_cell_w * 2;
int h_2_top = grid_cell_w * 5;
int h_3_top = grid_cell_w * 12;
int h_4_top = grid_cell_w * 16;
int h_5_top = window_h - grid_cell_w * 2;
int h_6_top = window_h - grid_cell_w * 6;
int h_7_top = window_h - grid_cell_w * 11;
int button_top = window_h / 2 + grid_cell_w * 4;

int window_w = 1600, window_h = 900;
int grid_cell_w = 30;
int FVR = 3;
int pf_border_w = 1;
int pf_w = 9;
int pf_h = 20 + FVR;
int pf_left = (0.5 * window_w) - (0.5 * pf_w * grid_cell_w);
int pf_right = pf_left + pf_w * grid_cell_w;
int pf_top  = (0.5 * window_h) - (0.5 * (pf_h - FVR) * grid_cell_w);
int pf_bottom = pf_top + (pf_h - FVR) * grid_cell_w;
int pp_left = pf_right + grid_cell_w * 2;
int pp_top = pf_top + ((FVR - 2) * grid_cell_w);
int hp_left = pf_left - grid_cell_w * 6;
int hp_top = pf_top + ((FVR - 2) * grid_cell_w);

int sm_font_size = grid_cell_w * 0.75; 
int med_font_size = grid_cell_w;
int lg_font_size = grid_cell_w * 2;

int min_time_step = 17;
int DAS_long = 16;
int DAS_short = 6;
int ticks_between_falls = gravity[start_level];
int ticks_between_falls_sd = ticks_between_falls / 2;
int tick_counter = 0;

int cur_level = start_level;
int lines_cleared_to_advance;
int LCTA_inc;
int score = 0;
GameData save_data;

char EMPTY_CELL   = ' ';
char RED_CELL     = 'R';
char GREEN_CELL   = 'G';
char BLUE_CELL    = 'B';
char CYAN_CELL    = 'C';
char MAGENTA_CELL = 'M';
char YELLOW_CELL  = 'Y';
char ORANGE_CELL  = 'O';
char WHITE_CELL   = 'W';

array<Uint8, 3> black    = {0, 0, 0};
array<Uint8, 3> drk_grey = {100, 100, 100};
array<Uint8, 3> med_grey = {150, 150, 150};
array<Uint8, 3> lgt_grey = {200, 200, 200};
array<Uint8, 3> white    = {255, 255, 255};
array<Uint8, 3> red      = {127, 0, 0};
array<Uint8, 3> green    = {0, 127, 0};
array<Uint8, 3> blue     = {70, 70, 200};
array<Uint8, 3> cyan     = {0, 127, 127};
array<Uint8, 3> magenta  = {127, 0, 127};
array<Uint8, 3> yellow   = {127, 127, 0};
array<Uint8, 3> orange   = {150, 80, 0};
array<Uint8, 3> drk_red  = {120, 80, 80};

unordered_map<char, array<Uint8, 3>> GCCM = {{
	{RED_CELL, red},
	{GREEN_CELL, green},
	{BLUE_CELL, blue},
	{CYAN_CELL, cyan},
	{MAGENTA_CELL, magenta},
	{YELLOW_CELL, yellow},
	{ORANGE_CELL, orange},
	{WHITE_CELL, white}
}};

SDL_Window* window = SDL_CreateWindow(window_caption, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_w, window_h, SDL_WINDOW_SHOWN);
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
