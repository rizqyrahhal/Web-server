#ifndef WEB_SERV_HPP
# define WEB_SERV_HPP

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
#include <arpa/inet.h>
#include <algorithm>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

class locations
{
public:

	locations(/* args */){}
	std::string name;
	std::string root;
	std::vector<std::string> allow_methods;
	std::string autoindex;
	std::map<std::string, std::string> cgi;
	std::map<int, std::string> redirect;

	~locations(){}
};



class request
{
	public:
	request(){}
	void	read_reqwest(int fd_client);
	std::string method;
	std::string url;
	std::map<std::string, std::string> map_request;
	~request(){}
	
};

class client
{

	public:
	client(/* args */);

	int fd_client;
	struct sockaddr_in client_address;
	socklen_t clientaddrlenght;
	request request_client;

	~client();
};

class server
{
	public:
	server(){}
	
	int port;
	std::string ip_address;
	std::string server_name;
	std::vector<client> client;
	std::vector<locations> locations;
	std::map<int, std::string> map_err_page;
	int client_body_size;

	struct sockaddr_in addr;
	struct addrinfo hints;
	struct addrinfo *res;
	int fd_server;
	static fd_set current; 
	unsigned int sizeof_struct;
	static int maxfd;

	static fd_set initializer();
	~server(){}
};

class global
{
public:
	global(){}
	~global(){}
	std::vector<server> server;
};


void    ft_parce_config(char **av, global &global);
void    creat_socket_and_bind(global & glob);
void    listen_new_connection(global & glob);
void    run_servers(global & glob);

# endif