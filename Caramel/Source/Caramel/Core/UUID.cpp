#include "clpch.h"
#include "UUID.h"

#include <random>

namespace Caramel
{
	static std::random_device s_randDevice;
	static std::mt19937_64 s_randEngine(s_randDevice());
	static std::uniform_int_distribution<uint64_t> s_randUniformDist;


	UUID::UUID() : m_id(s_randUniformDist(s_randEngine))
	{
	}

	UUID::UUID(uint64_t id) : m_id(id)
	{
	}
}
