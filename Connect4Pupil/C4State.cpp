#include "C4State.h"

std::vector<int> C4State::legal_actions() const
{
	std::vector<int> actions;
	for (int x = 0; x < param::W; ++x) {
		for (int y = param::H - 1; y >= 0; --y) {
			if (m_my_board[y][x] == 0 && m_emy_board[y][x] == 0) {
				actions.emplace_back(x);
				break;
			}
		}
	}
	return actions;
}

void C4State::advance(int in_action)
{
	Vec2i coordinate; //置いた座標
	for (int y = 0; y < param::H; ++y) {
		if (m_my_board[y][in_action] == 0 && m_emy_board[y][in_action] == 0) {
			m_my_board[y][in_action] = 1;
			coordinate = Vec2i(in_action, y);
			break;
		}
	}

	auto lambda_check_connect = [this](const auto& ofslst, const Vec2i& pos) {
		auto que = std::deque<Vec2i>();
		que.emplace_back(pos);
		std::vector<std::vector<bool>> check(param::H, std::vector<bool>(param::W, false));
		int count = 0;
		while (!que.empty()) {
			const auto tmp_cood = que.front(); //copy
			que.pop_front();
			++count;
			if (count >= 4) {
				// 自分の駒が揃ったら相手視点負け
				m_winning_stat = WinningStatus::LOSE;
				break;
			}
			check[tmp_cood.m_y][tmp_cood.m_x] = true;

			for (const auto d : ofslst) { //1,-1とか
				const auto t = tmp_cood + d;
				if (t.is_valid() && m_my_board[t.m_y][t.m_x] && !check[t.m_y][t.m_x]) {
					que.emplace_back(t);
				}
			}
		}
	};

	// 横方向の連結判定
	{
		constexpr std::array<Vec2i, 2> delta_row = { Vec2i(1,0), Vec2i(-1,0) };
		lambda_check_connect(delta_row, coordinate);
	}
	// "／"方向の連結判定
	if (!this->is_done()) {
		constexpr std::array<Vec2i, 2> dright_up = { Vec2i(1,1), Vec2i(-1,-1) };
		lambda_check_connect(dright_up, coordinate);
	}
	// "＼"方向の連結判定
	if (!this->is_done()) {
		constexpr std::array<Vec2i, 2> dleft_up = { Vec2i(1,-1), Vec2i(-1,1) };
		lambda_check_connect(dleft_up, coordinate);
	}
	// 縦方向の連結判定
	if (!this->is_done()) {
		auto tp = coordinate;
		bool is_win = true;
		for (int i = 0; i < 4; ++i) {
			const bool is_mine = (tp.is_valid() && m_my_board[tp.m_y][tp.m_x] == 1);
			if (!is_mine) {
				is_win = false;
				break;
			}
			tp.m_y -= 1;
		}
		if (is_win) {
			// 自分の駒が揃ったら相手視点負け
			m_winning_stat = WinningStatus::LOSE;
		}
	}

	// 入れ替え
	std::swap(m_my_board, m_emy_board);
	m_turn += 1;
	m_is_first = !m_is_first;
	if (!this->is_done() && this->legal_actions().size() == 0) {
		m_winning_stat = WinningStatus::DRAW;
	}
}

std::string C4State::to_string() const
{
	std::stringstream ss("");

	ss << "is_first:\t" << m_is_first << "\n";
	for (int y = param::H - 1; y >= 0; --y) {
		for (int x = 0; x < param::W; x++) {
			char c = '.';
			if (m_my_board[y][x] == 1) {
				c = (m_is_first ? 'x' : 'o');
			}
			else if (m_emy_board[y][x] == 1) {
				c = (m_is_first ? 'o' : 'x');
			}
			ss << c;
		}
		ss << "\n";
	}

	return ss.str();
}
