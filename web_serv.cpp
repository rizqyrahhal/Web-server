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
	char buffer[BUFFER_SIZE] = {0};
	read(fd_client, buffer, BUFFER_SIZE);
	std::cout<<buffer<<std::endl;
	std::cout<<"hello the file descriptor of this client is : "<<fd_client<<std::endl;
	std::string str = buffer;
	std::vector<std::string> request_line;
	std::stringstream tmp(str);
	std::string s;
	while(std::getline(tmp, s))
	{
		request_line.push_back(s);
	}
	std::stringstream tmp1(request_line[0]);
	tmp1 >> s;
	//check allow methods
	method = s;
	tmp1 >> s;
	if (s.lenght() > 2048)
		//respose 414
	//check for allow caracter
	std::string caracter = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
	for(int j = 0; j < s.lenght(); j++)
	{
		int f = caracter.find(s[i])
		if (f == caracter::npos())
			{
				//responce 404 bad request
			}
	}
	size_t found = s.find('?');
	if (found != std::string::npos){
		url = s.substr(0, found);
		query = s.substr(found + 1);
	}
	else {
		url = s;
	}
	std::string key;
	std::string value;
	for(size_t i = 1; i < request_line.size();i++)
	{
		int sep = request_line[i].find(":");
		key = request_line[i].substr(0, sep);
		value = request_line[i].substr(sep + 1);
		map_request.insert(std::make_pair(key, value));
	}
	std::cout<<"*********************************************************\n";
}


fd_set server::initializer()
{
	FD_ZERO(&current);
	return (current);
}