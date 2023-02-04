/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs350_1
Author: Junwoo Seo, junwoo.seo, 0055213
End Header --------------------------------------------------------*/
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
