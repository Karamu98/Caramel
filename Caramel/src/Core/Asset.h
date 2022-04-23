#pragma once
namespace Caramel
{
	class Asset
	{
	public:
		Asset();
		bool IsValid() { return m_isValid; }
	protected:
		bool m_isValid;
	};
}


