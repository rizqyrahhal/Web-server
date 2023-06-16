#include "web_serv.hpp" 

client::client()
{
	clientaddrlenght = sizeof(client_address);
}

client::~client()
{

}


fd_set server::initializer()
{
	FD_ZERO(&current);
	return (current);
}