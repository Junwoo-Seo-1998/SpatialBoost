#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs350_1
Author: Junwoo Seo, junwoo.seo, 0055213
End Header --------------------------------------------------------*/
#include <unordered_map>
#include <stdint.h>

class UUID
{
public:
	operator uint64_t() const { return m_UUID; }
	UUID();
	UUID(uint64_t uuid);
	UUID(const UUID&) = default;
	static constexpr uint64_t null = 0;
private:
	uint64_t m_UUID;
};

namespace std
{
	template<>
	struct hash<UUID>
	{
		std::size_t operator()(const UUID& uuid) const
		{
			return hash<uint64_t>()(static_cast<uint64_t>(uuid));
		}
	};
}
