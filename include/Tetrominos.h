#pragma once

#include "GlobalVariables.h"

#include <vector>
#include <string>
#include <array>

using namespace std;
using pf_t = vector<string>;
using tetromino_t = vector<vector<string>>;
// A list holding all of the permutaions (distinct orientations) of a given tetromino

extern array<tetromino_t, 7> tetrominos;