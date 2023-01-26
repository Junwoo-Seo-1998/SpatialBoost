#include "UUID.h"
#include <random>

namespace UUID_Engine
{
	static std::random_device random_device{};
	static std::mt19937_64 engine{ random_device()};
	static std::uniform_int_distribution<uint64_t> uniform_int_distribution{};
}


UUID::UUID()
	:m_UUID(UUID_Engine::uniform_int_distribution(UUID_Engine::engine))
{
}

UUID::UUID(uint64_t uuid)
	:m_UUID(uuid)
{
}
