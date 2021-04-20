#pragma once

#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <Winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <functional>
#include <thread>
#include <mutex>
#include <list>
#include <string>
#include <vector>
#include <algorithm>

const int buf_size = 4096;

class TCPServer
{
public:
	class TCPClient;

	TCPServer(unsigned port, std::function<void(TCPClient&)> callback);
	~TCPServer();

	void start();

private:
	unsigned port;
	SOCKET S; //дескриптор прослушивающего сокета
	SOCKET NS;
	sockaddr_in serv_addr;
	WSADATA wsadata;
	std::function<void(TCPClient&)> callback;
	std::list<std::thread> threads;
	std::list<std::shared_ptr<TCPClient>> clients;
	std::mutex print_mutex;
	std::mutex vec_mutex;
	std::vector<size_t> del;

	void client_loop(std::shared_ptr<TCPClient> client);
};

class TCPServer::TCPClient
{
public:
	TCPClient(SOCKET S, sockaddr_in cli_addr);
	~TCPClient();

	const std::string& get_data();
	void send_data(const std::string& text);

	friend class TCPServer;

private:
	SOCKET S; //сокет клиента
	sockaddr_in cli_addr;
	std::string data;

	const std::string& get_data(bool& err);
};