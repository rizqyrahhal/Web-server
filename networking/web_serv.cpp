#include "web_serv.hpp" 

request::request(size_t max_size)
{
	this->max_body_size = max_size;
}

request::~request()
{
	
}

int request::parce_header(std::string header)
{
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
	if (s.length() > 2048)
		return(414);	//check for allow caracter
	std::string caracter = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
	for(size_t j = 0; j < s.length(); j++)
	{
		size_t f = caracter.find(s[j]);
		if (f == std::string::npos)		{
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
	// std::cout<<"parcing request"<<std::endl;
	std::string key;
	std::string value;
	for(size_t i = 1; i < request_line.size();i++)
	{
		int sep = request_line[i].find(":");
		key = request_line[i].substr(0, sep);
		int r = request_line[i].find("\r");
		value = request_line[i].substr(sep + 2, r - (sep + 2));
		map_request.insert(std::make_pair(key, value));
	}

	if (map_request.count("Transfer-Encoding") > 0 && map_request["Transfer-Encoding"] != "chunked")
		return(501);
	if (method == "Post" && map_request.count("Transfer-Encoding") <= 0 && map_request.count("Content-Length") <= 0)
		return(400);
	if (map_request.count("Content-Length") > 0)
	{
		std::stringstream op;
		op << (map_request["Content-Length"]);
		op >> lenght;
		if (lenght > max_body_size)
			return(413);
	}
	// std::cout<<"length original : "<<lenght<< std::endl;
	return (0);
}



void request::parce_chunks(std::string body, int ffd, client & client)
{
	size_t i = 0;
	std::cout<<body.size()<<std::endl;
	body2 = body;
	
	while (i < body.size())
	{
		if (sizehex == 0)
		{
			int rn = body2.find("\r\n");
			if (rn == 0) {
				body2 = body2.substr(2);
				rn = body2.find("\r\n");
			}
			std::cout<<"rn == "<<rn<< " aa == " << body2[0] << std::endl;
			std::string hexa = body2.substr(0, rn);
			size_hexa_string = hexa.size();
			std::stringstream hex;
			hex << std::hex <<hexa;
			hex >> sizehex;
			// std::cout<<"hexa == "<<sizehex<<std::endl;
			if (sizehex == 0)
				{
					// std::cout<<"saliinaa mn chunked\n";
					close(bodyFile);
					client.check = 1;
					return ;
				}
			body2 = body2.substr(rn + 2);
			
		}
		else if (body2.size() < sizehex && add_it_to_body == 0)
		{
			// std::cout<<"salam khouya ana sgher mn sizehexa"<<std::endl;
			write(ffd, body2.c_str(), body2.size());
			sizehex -= body2.size();
			return ;
		}
		// tal hna mzyaaaan
		else if (body2.size() >= sizehex)
		{
			if (add_it_to_body == 0)
			{
				std::string tomp = body2.substr(sizehex);
				int fnd = tomp.find("\r\n");
				if (fnd != -1)
				{
					tomp = tomp.substr(2);
					int f2 = tomp.find("\r\n");
					if (f2 != -1)
					{
						write(ffd, body2.c_str(), sizehex);
						body2 = body2.substr(sizehex + 2);
						// std::cout << "body size 333:"  << body2.size()  << " body = |" << body2 << "|" << "\n";
						i += sizehex + 2;
						// std::cout << " i == " << i << " size: " << body.size() << std::endl;
						sizehex = 0;
					}
					else
					{
						write(ffd, body2.c_str(), sizehex);
						body2 = body2.substr(sizehex + 2);
						// i += sizehex + 2;
						sizehex = 1;
						add_it_to_body = 1;
						string_to_add = tomp;
						return ;
					}
				}
				else 
				{
					write(ffd, body2.c_str(), sizehex);
					if (sizehex == body2.size()) {
						sizehex = 0;
					}
					else if (!tomp.empty())
					{
						string_to_add = tomp;
						add_it_to_body = 1;
						sizehex = 1;
					}
					
					return ;
				}
			}
			else{
				body2 = string_to_add + body2;
				if (body2[0] == '\r')
					body2 = body2.substr(2);
				sizehex = 0;
				add_it_to_body = 0;
				i += string_to_add.size();
			}
		}
	}
}

int request::read_reqwest(client & client, std::vector<server> & servers, int index_client)
{
	std::vector<char> buffer(2024); // Adding by rarahhal
	// std::cout<<"*********** before recive **************"<<std::endl;
	int bytesrecv = recv(client.fd_client, &buffer[0], 2024, 0); // Adding by rarahhal
	// std::cout<<"*********** after recive **************"<<std::endl;
	// std::cout<<"read request for client : "<<client.fd_client<<"  byte recive = : "<<bytesrecv<<std::endl;
	if (!bytesrecv)
	{
		// std::cout<<"return from byte recive if "<<std::endl;
		client.check = 1;
		return(0);
	}
	// std::cout<<"byte receive : "<< bytesrecv<<std::endl;
	if (client.header_parced)
	{
		sizehex = 0;
		add_it_to_body = 0;
		lenght = 0;
		std::string str1(buffer.begin(), buffer.begin() + bytesrecv);
		int found1 = str1.find("\r\n\r\n", 0);
		std::string header = str1.substr(0, found1);
		std::cout<<"header : ***********************************************"<<std::endl;
		std::cout<<header<<std::endl;
		std::string body = std::string(str1.c_str() + found1 + 4, bytesrecv - (found1 + 4));
		std::cout<<"body : ***********************************************"<<std::endl;
		std::cout<<body<<std::endl;
		// std::string str(buffer.begin(), buffer.end());
		int staticcode = parce_header(header);
		if (staticcode > 0)
		{
			client.check = 1;
			return (staticcode);
			// std::cout<<"kayn chi mochkil f request"<<std::endl;
			// exit(0);
		}
		std::string host =  map_request["Host"];
		for (size_t i = 1; i < servers.size(); i++)
		{
			if (servers[i].server_name == host)
			{
				servers[0].client.erase(servers[0].client.begin() + index_client);
				servers[i].client.push_back(client);
				break;
			}
		}
		//check which method
		if (method != "GET" && method != "DELETE")
		{
			std::string filename;
			filename = "./upload/file";
			int gen = 0;
			while (access(filename.c_str(), F_OK) != -1)
			{
				filename = filename + std::to_string(gen);
				gen++;
			}
			bodyfile_name = filename;
			bodyFile = open(filename.c_str(),O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
			if (bodyFile == -1)
			{
				// std::cout<<"error !!"<<std::endl;
				exit(0);
			}
			// std::cout<<"ffd = "<<ffd<<std::endl;
			// std::cout<<"bodydd receive : "<< body.size()<< std::endl;
			if (map_request["Transfer-Encoding"] == "chunked")
			{
				// std::cout << "write : " << write(ffd, body.c_str(), body.size()) << std::endl;
				// std::cout<<"ana f chchunked"<<std::endl;
				parce_chunks(body, bodyFile, client);
				client.header_parced = false;
			}
			else{
				write(bodyFile, body.c_str(), body.size());
				client.header_parced = false;
				lenght = lenght - body.size();
				// std::cout<<"lenght first == "<<lenght<<std::endl;
				if (lenght <= 0)
				{
					// std::cout<<"byte recive < 1024"<<std::endl;
					close(bodyFile);
					client.check = 1;
				}
			}
		}
		else
		{
			// std::cout<<"saliinaa mn get\n";
			client.check = 1;
		}
		// std::cout<<"return chi haja"<<::std::endl;
		return (0);
	}
	// i finish the header parcing and i go working for body ...
	std::string str2(buffer.begin(), buffer.begin() + bytesrecv);

	if (map_request["Transfer-Encoding"] == "chunked")
	{
		// std::cout<<"ana f chchunked 3awtany *_*"<<std::endl;
		parce_chunks(str2, bodyFile, client);
	}
	else{
		write(bodyFile, str2.c_str(), bytesrecv);
		lenght = lenght - bytesrecv;
		// std::cout<<"lenght loop == "<<lenght<<std::endl;
		// std::cout<<"ffd after write"<<bodyFile<<std::endl;
  		if (lenght <= 0)
		{
			// std::cout<<"saliinaa\n";
			close(bodyFile); // Close the file
			client.check = 1;
		} // Append the string to the file

	}
	// std::cout<<str2<<std::endl;


	return (0);
}


fd_set server::initializer()
{
	FD_ZERO(&current);
	FD_ZERO(&current2);
	return (current);
}
