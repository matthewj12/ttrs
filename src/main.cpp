#include "GlobalVariables.h"
#include "GlobalFunctions.h"
#include "Piece.h"
#include "Tetrominos.h"
#include "Text.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

void pauseLoop();
void gameLoop();
void mainMenuLoop();
void settingsLoop();
void customPieceSeqLoop();
void customLevelProgLoop();
void highScoresLoop();
void secretLoop();
void loadData();
void saveData();

int main(int argc, char *args[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_RaiseWindow(window);
	TTF_Init();
	srand(static_cast<unsigned int>(time(nullptr)));
	getRandInt(0, 6);

	cur_screen_func_ptr = &gameLoop;
	// Settings* temp_settings = new Settings(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8]);
	// save_data.m_SS_map[temp_settings->hash()] = {10, 5, 2, 1, 1};

	// saveData();

	// save_data.m_SS_map.clear();

	loadData();

	cout << (cur_screen_func_ptr == &settingsLoop) << endl;

	while (program_running) {
		if (cur_screen_func_ptr != nullptr) {
			(*cur_screen_func_ptr)();
		}
		else {
			// pass
		}
	}

	saveData();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
	return 0;
}

