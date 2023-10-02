#pragma once

class TimeKeeper {
public:
	// 時間制限
	TimeKeeper(const int64_t& time_threshold)
		: m_start_time(std::chrono::high_resolution_clock::now())
		, m_time_threshold(time_threshold)
	{
	}

	// インスタンス生成した時から指定した時間制限を超過したか判定する。
	bool is_time_over() const
	{
		auto diff = std::chrono::high_resolution_clock::now() - m_start_time;
		return std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() >= m_time_threshold;
	}

private:
	std::chrono::high_resolution_clock::time_point m_start_time;
	int64_t m_time_threshold;
};
