#include "clpch.h"
#include "ArgumentParser.h"


namespace Caramel
{
	ArgumentParser* ArgumentParser::S_Instance = nullptr;

	ArgumentParser::ArgumentParser(int argc, char* argv[])
	{
		S_Instance = this;

		for (int i = 0; i < argc; ++i)
		{
			std::string current = argv[i];

			if (current.substr(0, 1) == "-")
			{
				size_t equalPos = current.find('=');
				if (equalPos != std::string::npos) 
				{
					std::string flag = current.substr(0, equalPos);
					std::string value = current.substr(equalPos + 1);
					m_setFlags[flag] = value;
				}
				else {
					m_setFlags[current] = ""; 
				}
			}
		}
	}

	bool ArgumentParser::GetFlag(const std::string& targetFlag, std::string* outValue)
	{
		bool found = S_Instance->m_setFlags.find(targetFlag) != S_Instance->m_setFlags.end();
		if (outValue != nullptr && found)
		{
			*outValue = S_Instance->m_setFlags[targetFlag];
		}
		return found;
	}
}
