#include "ModeGame.h"

#include "C4Game.h" //FIXME:
#include "C4AI.h"

ModeGame::ModeGame()
{
	m_tokens_tex[0] = Texture{ U"🔴"_emoji };
	m_tokens_tex[1] = Texture{ U"🟡"_emoji };
}

void ModeGame::update(bool is_advance, bool is_step)
{
	if (!is_advance) return;
	if (!m_state.is_done()) {
		auto ais = std::array<StringAIPair, 2>{
			StringAIPair("mctsActionWithTimeThreshold 1ms", [](const C4State& state) { return ai::montecarlo::mcts_action(state, 1); }),
			StringAIPair("mctsActionWithTimeThreshold 1ms", [](const C4State& state) { return ai::montecarlo::mcts_action(state, 1); }),
			//StringAIPair("randomAction", [](const C4State& state) { return C4Game::random_action(state); }),
			//StringAIPair("randomAction", [](const C4State& state) { return C4Game::random_action(state); }),
		};
		auto& first_ai = ais[0];
		auto& second_ai = ais[1];
		for (int k = 0;; k++) {
			m_state.advance(first_ai.second(m_state));
			if (m_state.is_done()) break;
			m_state.advance(second_ai.second(m_state));
			if (m_state.is_done()) break;
			if (is_step) return;
		}
	} else {
		m_state = C4State();
	}
}

void ModeGame::draw() const
{
	constexpr int32_t slot_r = 40;
	constexpr double gap = 15.0;
	constexpr double interval = slot_r*2.0 + gap;
	double ofsx = (param::SCRW - (param::W * slot_r * 2.0 + (param::W-1) * gap))/2.0 + slot_r;
	double ofsy = (param::SCRH - (param::H * slot_r * 2.0 + (param::H-1) * gap))/2.0 + slot_r;
	const bool is_first = m_state.is_first();
	for (int y = param::H - 1; y >= 0; --y) {
		for (int x = 0; x < param::W; x++) {
			Circle{ {ofsx + x * interval, ofsy + y * interval}, slot_r }.drawFrame(2, 2, Palette::Blue);

			int c = -1;
			if (m_state.get_my_board({ x,y })) {
				c = (is_first ? 1 : 0);
			}
			else if (m_state.get_emy_board({x,y})) {
				c = (is_first ? 0 : 1);
			}
			if (c >= 0) {
				const auto dy = param::H - y - 1;
				m_tokens_tex[c].scaled(0.54).drawAt(ofsx + x * interval, ofsy + dy * interval);
			}
		}
	}
}
