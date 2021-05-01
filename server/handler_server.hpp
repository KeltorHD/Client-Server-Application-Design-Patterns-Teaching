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
	std::string auth(const std::string& login, const std::string& password) const;
	std::string reg(const std::string& login, const std::string& password, const std::string& img_type, const std::string& img);
	std::string patterns() const;
	std::string result(const std::string& login, const std::string& password, const std::string& pattern, const std::string& result);
	std::string set_all_result(const std::string& login, const std::string& password, tinyxml2::XMLElement* body);

	bool is_correct_auth(const std::string& login, const std::string& password) const;
	std::string uncorrect() const;
	std::string correct() const;
	std::string encode_file(const std::string& path) const;
	void decode_file(const std::string& path, const std::string& data);
	std::vector<std::string> delim(const std::string& str, const std::string& delim) const;
};