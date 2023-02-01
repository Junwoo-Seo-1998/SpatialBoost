#pragma once
#include <unordered_map>

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
