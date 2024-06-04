#pragma once

namespace Caramel
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f) : m_time(time) {}

		inline float GetSeconds() const { return m_time; }
		inline float GetMilliseconds() const { return m_time * 1000.0f; }

		operator float() const { return m_time; }

	private:
		float m_time;
	};
}


