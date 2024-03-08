#pragma once
#include "clpch.h"

namespace Caramel
{
	class ArgumentParser
	{
	public:
		ArgumentParser(int argc, char* argv[]);

		static bool GetFlag(const std::string& targetFlag, std::string* outValue = nullptr);


	private:
		static ArgumentParser* S_Instance;

		std::unordered_map<std::string, std::string> m_setFlags;
	};
}


