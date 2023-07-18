#ifndef CLIENT_HPP
# define CLIENT_HPP

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
#include <stdlib.h>

#include "web_serv.hpp"
#include "../response/includes/ResponseReturned.hpp"



#define BUFFER_SIZE 1024

class request;
class ResponseReturned;

class client
{

	public:
	client(/* args */);
	bool header_parced;
	int fd_client;
	int bytesrecv;
	struct sockaddr_storage client_address;
	socklen_t clientaddrlenght;
	int check;
	int p;
	int resp;
	int pr;
	int readFd;
	int max_client_body_size;
	request *request_client;
	ResponseReturned response_client;
	bool generateResponseObject;

	// unsigned long size;
	client(int maxBodySize);
	bool siftna_response;
	~client();
};



# endif