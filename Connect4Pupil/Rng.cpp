#include "Rng.h"

namespace rng {
	std::mt19937 mt_for_action(0); //seed0

	uint32_t for_action()
	{
		return mt_for_action();
	}
}
