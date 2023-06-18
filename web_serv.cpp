#include "web_serv.hpp" 

client::client()
{
	clientaddrlenght = sizeof(client_address);
}

client::~client()
{

}

void request::read_reqwest(int fd_client)
{
	std::cout<<fd_client<<std::endl;
}


fd_set server::initializer()
{
	FD_ZERO(&current);
	return (current);
}