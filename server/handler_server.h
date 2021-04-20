#pragma once

#include <string>

class Handler_server
{
public:
	Handler_server() = default;
	~Handler_server() = default;

	std::string operator()(const std::string& data)
	{
		return "Hello\r\n";
	}

private:



};