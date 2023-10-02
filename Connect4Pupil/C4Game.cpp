#include "C4Game.h"

#include <Siv3D.hpp> // Siv3D v0.6.12
#include "C4State.h"
#include "Rng.h"

int C4Game::random_action(const C4State& state)
{
	auto legal_actions = state.legal_actions();
	return legal_actions[rng::for_action() % legal_actions.size()];
}

void C4Game::play_game()
{
	Console.open(); //consoleでcout表示される
	using std::cout;
	using std::endl;

	auto state = C4State();
	cout << state.to_string() << endl;
	while (!state.is_done())
	{
		// 1p
		{
			cout << "1p ------------------------------------" << state.get_turn() << endl;
			int action = random_action(state);
			cout << "action " << action << endl;
			state.advance(action); // (a-1) ここで視点が入れ替わり、2p視点になる。
			cout << state.to_string() << endl;
			if (state.is_done()) {
				switch (state.get_winning_stat()) { // (a-2) a-1で2P視点になっているので、WINなら2pの勝利
				case (WinningStatus::WIN): cout << "winner: 2p" << endl; break;
				case (WinningStatus::LOSE):	cout << "winner: 1p" << endl; break;
				default: cout << "DRAW" << endl; break;
				}

				break;
			}
		}
		// 2p
		{
			cout << "2p ------------------------------------" << state.get_turn() << endl;
			int action = random_action(state);
			cout << "action " << action << endl;
			state.advance(action); // (b-1) ここで視点が入れ替わり、1p視点になる。
			cout << state.to_string() << endl;
			if (state.is_done()) {
				switch (state.get_winning_stat()) { // (b-2) b-1で2P視点になっているので、WINなら1pの勝利
				case (WinningStatus::WIN): cout << "winner: 1p" << endl; break;
				case (WinningStatus::LOSE):	cout << "winner: 2p" << endl; break;
				default: cout << "DRAW" << endl; break;
				}

				break;
			}
		}
	}
}

void C4Game::test_first_plyaer_win_rate(const std::array<StringAIPair, 2>& ais, const int game_number)
{
	Console.open(); //consoleでcout表示される
	using std::cout;
	using std::endl;

	double first_player_win_rate = 0;
	for (int i = 0; i < game_number; i++)
	{
		auto base_state = C4State();
		for (int j = 0; j < 2; j++)
		{ // 先手後手平等に行う
			auto state = base_state;
			auto& first_ai = ais[j];
			auto& second_ai = ais[(j + 1) % 2];
			for (int k = 0;; k++)
			{
				state.advance(first_ai.second(state));
				if (state.is_done())
					break;
				state.advance(second_ai.second(state));
				if (state.is_done())
					break;
			}
			double win_rate_point = state.get_first_player_score_for_win_rate();
			if (j == 1)
				win_rate_point = 1 - win_rate_point;
			if (win_rate_point >= 0)
			{
				state.to_string();
			}
			first_player_win_rate += win_rate_point;
		}
		cout << "i " << i << " w " << first_player_win_rate / ((i + 1) * 2) << endl;
	}
	first_player_win_rate /= (double)(game_number * 2);
	cout << "Winning rate of " << ais[0].first << " to " << ais[1].first << ":\t" << first_player_win_rate << endl;
}
