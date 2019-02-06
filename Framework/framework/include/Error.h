#pragma once
#ifndef __ERROR_H__
#define __ERROR_H__



#include <sstream>
#include <string>



class Error
{
public:

	static void RuntimeError(std::string a_error)
	{
		m_lastError.clear();
		m_lastError << a_error;
	}

	static std::string GetLastRuntimeError()
	{
		return m_lastError.str();
	}

private:
	static std::ostringstream m_lastError;

};

#endif //__ERROR_H__
