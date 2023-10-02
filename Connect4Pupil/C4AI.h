#pragma once
class C4State;
namespace ai {
	namespace montecarlo {
		int mcts_action(const C4State& state, const int64_t time_threshold);
	}
}
