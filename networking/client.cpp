#include "client.hpp"
#include "web_serv.hpp"

client::client()
{
	clientaddrlenght = sizeof(client_address);
	check = 0;
}

client::~client()
{
	// delete this->request_client;
}

client::client(int maxClientBodySize) {
	clientaddrlenght = sizeof(client_address);
	this->max_client_body_size = maxClientBodySize;
	this->request_client = new request(maxClientBodySize);
	check = 0;
	p = 0;
	readFd = -1;
	// size = 0;
	resp = 0;
	pr = 0;
	header_parced = true;
	generateResponseObject = false;
} 