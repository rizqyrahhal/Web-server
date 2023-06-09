#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/select.h>
#include <stdio.h>
#include <sstream>
#include <map>

class locations
{
public:

	locations(/* args */){}
	std::string root;
	std::vector<std::string> allow_methods;
	std::string autoindex;
	std::map<std::string, std::string> cgi;
	std::map<int, std::string> redirect;

	~locations(){}
};

class client
{

public:
	client(/* args */){}

	int fd_client;

	~client(){}
};



class server
{
	public:
	server(){}
	~server(){}
	
	int port;
	std::string ip_address;
	std::string server_name;
	std::vector<client> client;
	std::vector<locations> locations;
	std::map<int, std::string> map_err_page;
	int client_body_size;
	int id_server;

};

class global
{
public:
	global(){}
	~global(){}
	std::vector<server> server;
};

void    ft_parce_config(char **av, global &global);