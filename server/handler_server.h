#pragma once

#include <tinyxml2.h>
#include <SQLiteCpp/SQLiteCpp.h>

#include <string>
#include <iostream>
#include <mutex>


class Handler_server
{
public:
	Handler_server() = default;
	~Handler_server() = default;

	std::string processing(const std::string& data);

	static void init_coonection_bd();

private:
	std::string auth(std::string login, std::string password);
	bool is_correct_auth(std::string login, std::string password);

	static std::mutex mutex;
};