#pragma once

namespace Caramel
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t id);

		operator uint64_t() const { return m_id; }
	private:
		uint64_t m_id;
	};
}

namespace std
{
	template<>
	struct hash<Caramel::UUID>
	{
		std::size_t operator()(const Caramel::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}