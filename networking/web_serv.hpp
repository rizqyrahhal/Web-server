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

#include "../response/includes/responsePart.hpp"

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

	std::string index;  //this line aadded by rarahhal
	std::map<int, std::string> map_err_page; /// this line added by rarahhal
	// need map to the error_page inside location

	~locations(){}
};



class request
{
	public:
	request(){}
	request(int max_size);
	int	read_reqwest(int fd_client);
	std::string method;
	std::string url;
	std::string query;
	int max_body_size;
	std::map<std::string, std::string> map_request;
	~request(){}	
};

class client
{

	public:
	client(/* args */);

	int fd_client;
	int bytesrecv;
	struct sockaddr_in client_address;
	socklen_t clientaddrlenght;
	int check;
	int max_client_body_size;
	request *request_client;
	client(int maxBodySize);
	~client();
};

class server
{
	public:
	server(){}
	
	int port;
	std::string ip_address;
	std::string server_name;
	std::string root;
	std::string index;
	std::vector<client> client;
	std::vector<locations> locations;
	std::map<int, std::string> map_err_page;
	int client_body_size;

	struct sockaddr_in addr;
	struct addrinfo hints;
	struct addrinfo *res;
	int fd_server;
	static fd_set current;
	static fd_set current2;
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