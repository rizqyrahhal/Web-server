#include "web_serv.hpp" 

request::request(int max_size)
{
	this->max_body_size = max_size;
}

int request::read_reqwest(client & client)
{
	std::vector<char> buffer(1024); // Adding by rarahhal
	int bytesrecv = recv(client.fd_client, &buffer[0], 1024, 0); // Adding by rarahhal
	std::cout<<"byte receive"<< bytesrecv<<std::endl;
	int ffd;
	if (client.header_parced)
	{
		std::string str1(buffer.begin(), buffer.end());
		int found1 = str1.find("\r\n\r\n", 0);
		std::string header = str1.substr(0, found1);
		std::cout<<"header : ***********************************************"<<std::endl;
		std::cout<<header<<std::endl;
		std::string body = str1.substr(found1 + 4);
		std::cout<<"body : ***********************************************"<<std::endl;
		std::cout<<body<<std::endl;
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
		// std::ofstream file("body_file.png");
		// if (file.is_open()) {
    	// 	file << body;// Append the string to the file
		// 	file.close(); // Close the file
    	// //    std::cout << "String appended to the file." << std::endl;
    	// } else {
    	//     std::cerr << "Failed to open the file." << std::endl;
		// }
		ffd = open("body_file.png", O_APPEND | O_WRONLY);
		write(ffd, body.c_str(), body.size());
		client.header_parced = false;
		return (0);
	}
	// i finish the header parcing and i go working for body ...
	std::string str2(buffer.begin(), buffer.end());
	std::cout<<str2<<std::endl;
	// std::ofstream file("body_file.png");
	// if (file.is_open()) {
	// 	std::cout<<"sift lba9iaaa"<<std::endl;
    	// file << str2;
		write(ffd, str2.c_str(), bytesrecv);
	  	if (bytesrecv < 1024)
		{
			std::cout<<"saliinaa\n";
			close(ffd); // Close the file
			client.check = 1;
		} // Append the string to the file
    //    std::cout << "String appended to the file." << std::endl;
    // } else {
    //     std::cerr << "Failed to open the file." << std::endl;
	// }
	return (0);
}


fd_set server::initializer()
{
	FD_ZERO(&current);
	FD_ZERO(&current2);
	return (current);
}