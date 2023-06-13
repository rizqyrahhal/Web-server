#include "web_serv.hpp" 

client::client()
{
	clientaddrlenght = sizeof(client_address);
}

client::~client()
{
	std::cout<<"destructer called"<<std::endl;
}