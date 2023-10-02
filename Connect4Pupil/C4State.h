#pragma once

enum class WinningStatus {
	WIN,
	LOSE,
	DRAW,
	NONE,
};

namespace param {
	inline constexpr int H = 6; //ボードの高さ
	inline constexpr int W = 7; //ボードの幅
	inline constexpr int64_t INF = 1000000000LL;
}

struct Vec2i {
	constexpr Vec2i() = default;
	constexpr Vec2i(int ix, int iy)
		: m_x(ix)
		, m_y(iy)
	{}
	int m_x = 0;
	int m_y = 0;

	bool is_valid() const { return (m_y >= 0 && m_y < param::H && m_x >= 0 && m_x < param::W); }
	Vec2i operator +(const Vec2i& r) const {
		return Vec2i(m_x + r.m_x, m_y + r.m_y);
	}
};

class C4State {
public:
	C4State() = default;

	bool is_done() const { return (m_winning_stat != WinningStatus::NONE); } //ゲーム終了か
	uint16_t get_turn() const { return m_turn; }
	WinningStatus get_winning_stat() const { return m_winning_stat; }
	std::vector<int> legal_actions() const;
	void advance(int in_action); //1ターン進め,次のプレイヤー視点の盤面にする
	double get_first_player_score_for_win_rate() const;
	std::string to_string() const;

private:
	bool m_is_first = true; //先手番の場合true
	uint16_t m_turn = 0;
	int m_my_board[param::H][param::W] = {};
	int m_emy_board[param::H][param::W] = {};
	WinningStatus m_winning_stat = WinningStatus::NONE;
};
