#include "tcpserver.hpp"
#include "handler_server.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>

int main()
{
	/*std::ifstream file("admin.png", std::ios::in | std::ios::binary);

	if (!file.is_open())
		throw std::exception("404 Not Found-f");

	file.seekg(0, file.end);
	size_t length = file.tellg();
	file.seekg(0, file.beg);

	std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());


	std::ofstream out_tmp("tmp.png");
	out_tmp.close();
	std::ofstream out("tmp.png", std::ios::out | std::ios::binary);
	for (size_t i = 0; i < data.size(); i++)
	{
		out << data[i];
	}*/

	/*std::ifstream file("images/users_images/user2.png", std::ios::in | std::ios::binary);

	if (!file.is_open())
		throw std::exception("404 Not Found-f");

	file.seekg(0, file.end);
	size_t length = file.tellg();
	file.seekg(0, file.beg);

	std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	std::cout << base64_encode(data) << std::endl;*/

	TCPServer server
	(
		20002, 
		[](TCPServer::TCPClient& client)
		{
			std::string data{ client.get_data() };
			Handler_server handler;			

			std::string recv{ handler.processing(data) };
			//std::cout << recv << std::endl;
			client.send_data(recv);
		}
	);

	server.start();


	return 0;
}