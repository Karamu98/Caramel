#pragma once

namespace Caramel
{
	struct COM_UUID
	{
		COM_UUID(UUID id) : Identifier(id) {}
		UUID Identifier;
	};
}