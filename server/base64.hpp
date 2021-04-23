#pragma once

#include <vector>
#include <string>

std::string base64_encode(const std::vector<uint8_t>& buf); /*зашифровка*/
std::vector<uint8_t> base64_decode(const std::string& encoded_string);  /*расшифровка*/