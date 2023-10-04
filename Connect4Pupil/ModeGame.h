﻿#pragma once
#include "C4State.h"

class ModeGame {
public:
	ModeGame();
	void update();
	void draw() const;
	bool is_finished() const { return m_state.is_done(); }

private:
	Texture m_tokens_tex[param::PLAYER_NUM];
	C4State m_state;
};
