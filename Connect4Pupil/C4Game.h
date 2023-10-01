#pragma once
#include "C4State.h"

class C4Game {
public:
	C4Game() = default;

	static int random_action(const C4State& state);
	static void play_game();

private:
};
