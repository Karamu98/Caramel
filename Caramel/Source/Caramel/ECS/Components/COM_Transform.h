#pragma once

namespace Caramel
{
	struct COM_Transform
	{
		glm::mat4 Transform;

		COM_Transform() = default;
		COM_Transform(const COM_Transform&) = default;
		COM_Transform(const glm::mat4& trans) : Transform(trans) {}

		inline glm::vec3& RefPosition() { return *(glm::highp_vec3*)(&Transform[3]); };

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};
}