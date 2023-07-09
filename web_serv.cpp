#include "web_serv.hpp" 

request::request(int max_size)
{
	this->max_body_size = max_size;
}

int request::read_reqwest(client & client)
{
	std::vector<char> buffer(2743); // Adding by rarahhal
	int bytesrecv = recv(client.fd_client, &buffer[0], 2743, 0); // Adding by rarahhal
	// char buffer[BUFFER_SIZE] = {0};
	// memset(&buffer,0,1024);
	// int bytesrecv = recv(fd_client, buffer, 1023, 0);
	// std::cout << "----->" << bytesrecv << std::endl;
	// std::cout<< buffer <<std::endl;
	// std::cout << "Request: (read this size = " << bytesrecv << ")\n" << std::string(buffer.begin(), buffer.end()) << std::endl;
	std::string str1(buffer.begin(), buffer.end());
	int found = str1.find("\r\n\r\n", 0);
	std::string header = str1.substr(0, found);
	std::cout<<"header : ***********************************************"<<std::endl;
	std::cout<<header<<std::endl;
	std::string body = str1.substr(found);
	std::cout<<"body : ***********************************************"<<std::endl;
	// std::cout<<body<<std::endl;
    if (bytesrecv > 0)
	{
		if (client.header_parced)
		{
			// std::string str(buffer.begin(), buffer.end());
			std::vector<std::string> request_line;
			std::stringstream tmp(header);
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
			std::cout<<"parcing request"<<std::endl;
			std::string key;
			std::string value;
			for(size_t i = 1; i < request_line.size();i++)
			{
				int sep = request_line[i].find(":");
				key = request_line[i].substr(0, sep);
				value = request_line[i].substr(sep + 1);
				map_request.insert(std::make_pair(key, value));
			}
			client.header_parced = false;
		}
		// i finish the header parcing and i go working for body ...
		std::cout<<body<<std::endl;
	}
	return (0);
}


fd_set server::initializer()
{
	FD_ZERO(&current);
	FD_ZERO(&current2);
	return (current);
}