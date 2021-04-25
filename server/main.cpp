#include "tcpserver.hpp"
#include "handler_server.hpp"

#include <string>

int main()
{
	TCPServer server
	(
		20002, 
		[](TCPServer::TCPClient& client)
		{
			std::string data{ client.get_data() };
			Handler_server handler;			

			client.send_data(handler.processing(data));
		}
	);

	server.start();

	return 0;
}