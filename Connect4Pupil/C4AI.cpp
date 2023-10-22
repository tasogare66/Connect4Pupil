#include "C4AI.h"
#include <assert.h>
#include "C4State.h"
#include "C4Game.h"
#include "TimeKeeper.h"

namespace ai {
namespace montecarlo {
	// ランダムプレイアウトをして勝敗スコアを計算する
	double playout(C4State* state)
	{ // const&にすると再帰中にディープコピーが必要になるため、高速化のためポインタにする。(constでない参照でも可)
		switch (state->get_winning_stat()) {
		case (WinningStatus::WIN): return 1.0;
		case (WinningStatus::LOSE):	return 0.0;
		case (WinningStatus::DRAW):	return 0.5;
		default:
			state->advance(C4Game::random_action(*state));
			return 1.0 - playout(state);
		}
	}

	inline constexpr double C = 1.0; // UCB1の計算に使う定数
	inline constexpr int EXPAND_THRESHOLD = 10; // ノードを展開する閾値

	// MCTSの計算に使うノード
	class Node {
	public:
		std::vector<Node> m_child_nodes;
		double m_n = 0.0;

		Node(const C4State& in_state)
			: m_state(in_state)
		{}

		// ノードの評価を行う
		double evaluate()
		{
			if (m_state.is_done()) {
				double value = 0.5;
				switch (m_state.get_winning_stat()) {
				case (WinningStatus::WIN): value = 1.0; break;
				case (WinningStatus::LOSE): value = 0.0; break;
				default: break;
				}
				m_w += value;
				m_n += 1.0;
				return value;
			}
			if (m_child_nodes.empty())
			{
				auto state_copy = m_state;
				const double value = playout(&state_copy);
				m_w += value;
				m_n += 1.0;
				if (m_n == EXPAND_THRESHOLD) {
					this->expand();
				}
				return value;
			} else {
				double value = 1.0 - this->next_child_node().evaluate();
				m_w += value;
				m_n += 1.0;
				return value;
			}
		}

		// ノードを展開する
		void expand()
		{
			auto legal_actions = m_state.legal_actions();
			m_child_nodes.clear();
			for (const auto action : legal_actions)
			{
				m_child_nodes.emplace_back(m_state);
				m_child_nodes.back().m_state.advance(action);
			}
		}

		// どのノードを評価するか選択する
		Node& next_child_node()
		{
			for (auto& child_node : m_child_nodes)
			{
				if (child_node.m_n == 0) return child_node;
			}
			double t = 0;
			for (const auto& child_node : m_child_nodes)
			{
				t += child_node.m_n;
			}
			double best_value = -param::INF;
			int best_action_index = -1;
			for (int i = 0; i < m_child_nodes.size(); i++)
			{
				const auto& child_node = m_child_nodes[i];
				double ucb1_value = 1. - child_node.m_w / child_node.m_n + C * std::sqrt(2. * std::log(t) / child_node.m_n);
				if (ucb1_value > best_value)
				{
					best_action_index = i;
					best_value = ucb1_value;
				}
			}
			return m_child_nodes[best_action_index];
		}

	private:
		C4State m_state;
		double m_w = 0.0;
	};

	// 制限時間(ms)を指定してMCTSで行動を決定する
	int mcts_action(const C4State& state, const int64_t time_threshold)
	{
		Node root_node = Node(state);
		root_node.expand();
		auto time_keeper = TimeKeeper(time_threshold);
		for (int cnt = 0;; cnt++)
		{
			if (time_keeper.is_time_over() && cnt >= 20) //20回保証
			{
				break;
			}
			root_node.evaluate();
		}
		auto legal_actions = state.legal_actions();

		int best_action_searched_number = -1;
		int best_action_index = -1;
		assert(legal_actions.size() == root_node.m_child_nodes.size());
		for (int i = 0; i < legal_actions.size(); i++)
		{
			int n = static_cast<int>(root_node.m_child_nodes[i].m_n);
			if (n > best_action_searched_number)
			{
				best_action_index = i;
				best_action_searched_number = n;
			}
		}
		return legal_actions[best_action_index];
	}
} //montecarlo
} //ai
