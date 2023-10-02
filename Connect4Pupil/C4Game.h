#pragma once
#include "C4State.h"

using AIFunction = std::function<int(const C4State&)>;
using StringAIPair = std::pair<std::string, AIFunction>;

class C4Game {
public:
	C4Game() = default;
	static int random_action(const C4State& state);
	static void play_game();
	static void test_first_plyaer_win_rate(const std::array<StringAIPair, 2>& ais, const int game_number);
};
