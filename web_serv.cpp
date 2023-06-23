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
	this->max_client_body_size = maxClientBodySize;
	this->request_client = new request(maxClientBodySize);
	check = 0;
}

request::request(int max_size)
{
	this->max_body_size = max_size;
}

int request::read_reqwest(int fd_client)
{
	char buffer[BUFFER_SIZE] = {0};
	int bytesrecv = recv(fd_client, buffer, 1024, 0);
	// std::cout << "----->" << bytesrecv << std::endl;
	// std::cout<< buffer <<std::endl;
    if (bytesrecv > 0)
	{
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
		std::cout<<s<<std::endl;
		if (s.length() > 20)
		{
			return(414);
		}
		//check for allow caracter
		std::string caracter = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
		for(size_t j = 0; j < s.length(); j++)
		{
			size_t f = caracter.find(s[j]);
			if (f == std::string::npos)			{
					// std::cout<<"heloo im here for somme issue"<<std::endl;
					// // std::string response = GenerateResponseFromStatusCode(404);
					// send(fd_client, GenerateResponseFromStatusCode(404).c_str(), GenerateResponseFromStatusCode(404).size(), 0);
					return (404);
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
	}
	return (0);
}


fd_set server::initializer()
{
	FD_ZERO(&current);
	FD_ZERO(&current2);
	return (current);
}