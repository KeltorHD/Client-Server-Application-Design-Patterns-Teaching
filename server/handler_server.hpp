#pragma once

#include <tinyxml2.h>
#include <SQLiteCpp/SQLiteCpp.h>

#include "base64.hpp"

#include <string>
#include <iostream>
#include <mutex>
#include <fstream>

class Handler_server
{
public:
	Handler_server() = default;
	~Handler_server() = default;

	std::string processing(const std::string& data);

private:
	std::string auth(const std::string& login, const std::string& password);
	std::string reg(const std::string& login, const std::string& password, const std::string& img_type, const std::string& img);
	std::string patterns();

	bool is_correct_auth(const std::string& login, const std::string& password);
	std::string uncorrect();
	std::string correct();
	std::string encode_file(const std::string& path);
	std::vector<std::string> delim(std::string str, std::string delim);

	static std::mutex mutex;
};