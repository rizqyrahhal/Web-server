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
	if (method != "GET" && method != "POST" && method != "DELETE")
		return (501);
	tmp1 >> s;
	if (s.length() > 2048)
		return(414);
	std::string caracter = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
	for(size_t j = 0; j < s.length(); j++)
	{
		size_t f = caracter.find(s[j]);
		if (f == std::string::npos)		{
				return (404);
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
	tmp1 >> s;
	version = s;
	if (version != "HTTP/1.1")
		return(400);
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
	std::map<std::string, std::string>::iterator it;
	if (map_request.find("Transfer-Encoding") != map_request.end() && map_request["Transfer-Encoding"] != "chunked")
		return(501);
	it = map_request.begin();
	if ((method == "POST" && map_request.find("Transfer-Encoding") == map_request.end() && map_request.find("Content-Length") == map_request.end()) || (method == "POST" && map_request.find("Content-Type") == map_request.end()))
		return(400);
	if (map_request.count("Content-Length") > 0)
	{
		std::stringstream op;
		op << (map_request["Content-Length"]);
		op >> lenght;
		if (lenght > max_body_size)
			return(413);
	}
	return (0);
}



void request::parce_chunks(std::string body, int ffd, client & client)
{
	size_t i = 0;
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
			std::string hexa = body2.substr(0, rn);
			size_hexa_string = hexa.size();
			std::stringstream hex;
			hex << std::hex <<hexa;
			hex >> sizehex;
			if (sizehex == 0)
				{
					close(bodyFile);
					client.check = 1;
					return ;
				}
			body2 = body2.substr(rn + 2);
			
		}
		else if (body2.size() < sizehex && add_it_to_body == 0)
		{
			if (write(ffd, body2.c_str(), body2.size()) <= -1)
			{
				close(bodyFile);
				client.check = 1;
				return ;
			}
			sizehex -= body2.size();
			return ;
		}
		// tal hna mzyaaaan
		else if (body2.size() >= sizehex)
		{
			if (add_it_to_body <= 0)
			{
				std::string tomp = body2.substr(sizehex);
				int fnd = tomp.find("\r\n");
				if (fnd != -1)
				{
					tomp = tomp.substr(2);
					int f2 = tomp.find("\r\n");
					if (f2 != -1)
					{
						if (write(ffd, body2.c_str(), sizehex) <= -1)
						{
							close(bodyFile);
							client.check = 1;
							return;
						}
						body2 = body2.substr(sizehex + 2);
						i += sizehex + 2;
						sizehex = 0;
					}
					else
					{
						if( write(ffd, body2.c_str(), sizehex) <= -1)
						{
							close(bodyFile);
							client.check = 1;
							return ;
						}
						body2 = body2.substr(sizehex + 2);
						sizehex = 1;
						add_it_to_body = 1;
						string_to_add = tomp;
						return ;
					}
				}
				else 
				{
					if (write(ffd, body2.c_str(), sizehex) <= -1)
					{
						close(bodyFile);
						client.check = 1;
						return ;
					}
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

int request::read_reqwest(client & client, std::vector<server> & servers)
{
	std::vector<char> buffer(2024); // Adding by rarahhal
	int bytesrecv = recv(client.fd_client, &buffer[0], 2024, 0); // Adding by rarahhal
	if (bytesrecv <= 0)
	{
		client.check = 1;
		return(0);
	}
	if (client.header_parced)
	{
		sizehex = 0;
		add_it_to_body = 0;
		lenght = 0;
		std::string str1(buffer.begin(), buffer.begin() + bytesrecv);
		int found1 = str1.find("\r\n\r\n", 0);
		std::string header;
		std::string body;
		if (found1 == -1)
			header = str1;
		else{
			header = str1.substr(0, found1);
			body = std::string(str1.c_str() + found1 + 4, bytesrecv - (found1 + 4));
		}
		int staticcode = parce_header(header);
		if (staticcode > 0)
		{
			client.check = 1;
			return (staticcode);
		}
		std::string host =  map_request["Host"];
		for (size_t i = 1; i < servers.size(); i++)
		{
			if (servers[i]._name == host)
			{
				client.client_in_serv = i;
				break;
			}
		}
		if (method != "GET" && method != "DELETE")
		{
			std::string filename;
			filename = "./www/file";
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
				std::cout<<"error !!"<<std::endl;
				exit(0);
			}
			if (map_request["Transfer-Encoding"] == "chunked")
			{
				parce_chunks(body, bodyFile, client);
				client.header_parced = false;
			}
			else{
				if (write(bodyFile, body.c_str(), body.size()) <= -1)
				{
					close(bodyFile);
					client.check = 1;
				}
				client.header_parced = false;
				lenght = lenght - body.size();
				if (lenght <= 0)
				{
					close(bodyFile);
					client.check = 1;
				}
			}
		}
		else
		{
			client.check = 1;
		}
		return (0);
	}
	std::string str2(buffer.begin(), buffer.begin() + bytesrecv);

	if (map_request["Transfer-Encoding"] == "chunked")
	{
		parce_chunks(str2, bodyFile, client);
	}
	else{
		if (write(bodyFile, str2.c_str(), bytesrecv) == -1)
		{
			close(bodyFile);
			client.check = 1;
		}
		lenght = lenght - bytesrecv;
  		if (lenght <= 0)
		{
			close(bodyFile);
			client.check = 1;
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

server::server()
{
	port.push_back("8080");
	ip_address = "127.0.0.1";
	_name = "serve00";
	root = "./www";
	client_body_size = 100000000000000;
	allow_methods.push_back("GET");
	allow_methods.push_back("POST");
	allow_methods.push_back("DELETE");
}
