#include <array>
#include <unordered_map>

using namespace std;

struct GameData {
	unordered_map<int, array<int, 5>> m_SS_map;
	// Maps settings to score
	GameData() {
		m_SS_map.reserve(1);
	}
};