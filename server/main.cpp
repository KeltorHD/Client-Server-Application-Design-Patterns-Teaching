#include "tcpserver.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>

#include "handler_server.h"



int main()
{
	/*std::ifstream file("admin.png", std::ios::in | std::ios::binary);

	if (!file.is_open())
		throw std::exception("404 Not Found-f");

	file.seekg(0, file.end);
	size_t length = file.tellg();
	file.seekg(0, file.beg);
	uint8_t* buf = new uint8_t[length];

	std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());


	std::ofstream out_tmp("tmp.png");
	out_tmp.close();
	std::ofstream out("tmp.png", std::ios::out | std::ios::binary);
	for (size_t i = 0; i < data.size(); i++)
	{
		out << data[i];
	}*/

	TCPServer server
	(
		20002, 
		[](TCPServer::TCPClient& client)
		{
			std::string data{ client.get_data() };
			Handler_server handler;

			client.send_data(handler(data));
		}
	);

	server.start();

	return 0;
}