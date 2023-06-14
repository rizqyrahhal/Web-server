/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server-side.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 17:33:12 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/14 18:23:45 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "switch-data.hpp"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>

#define PORT 4875

int main(void)
{
    struct sockaddr_in adderss;

	int new_socket;

	memset(adderss.sin_zero, '\0', sizeof(adderss.sin_zero));

	adderss.sin_family = AF_INET;
	adderss.sin_addr.s_addr = htonl(INADDR_ANY);
	adderss.sin_port = htons(PORT);
    
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        std::cout << "ERROR in creation socket\n";
	
	int reuse = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    	std::cout << "ERROR setting socket options\n";
	}

	if (bind(server_fd, (struct sockaddr *)&adderss, sizeof(adderss)) < 0)
		std::cout << "ERROR in binding socket\n";
	
	if (listen(server_fd, 10) < 0)
		std::cout << "ERROR in listen\n";

	long valread;

	std::ifstream file("videoplayback.mp4", std::ifstream::binary);

	if (!file.is_open())
	{
		std::cout << "Error in opening file\n";
		std::exit(-404);
	}
	file.seekg(0, std::ios::end);
    int videoLength = file.tellg();
    file.seekg(0, std::ios::beg);

	char uffer[videoLength/2];
	file.read(uffer, videoLength/2);
	char uffer2[videoLength/2];
	file.read(uffer2, videoLength/2);

	file.close();
	char headers[66] = "HTTP/1.1 200 OK\nContent-Type: video/mp4\nContent-Length: ";
	std::string header = headers + std::to_string(videoLength) + "\n\n";
	std::string response = header + uffer + uffer2;

	while (1)
	{
		std::cout << "\n++++++ Waiting for new connection +++++++\n\n";
		if ((new_socket = accept(server_fd, (struct sockaddr *)&adderss, (socklen_t*)&adderss)) < 0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}
		char buffer[30000] = {0};
		valread = read(new_socket, buffer, 30000);
		
		std::cout << buffer << std::endl;
		
		send(new_socket, header.c_str(), header.size(), MSG_SEND);
		send(new_socket, uffer, videoLength/2, MSG_SEND);
		send(new_socket, uffer2, videoLength/2, MSG_SEND);
		// write(new_socket, headers.c_str(), response.size());
		// write(new_socket, uffer, 3465);
		std::cout << "-------------------response message sent---------------\n";
		std::cout << response << std::endl;
		std::cout << "------------------------------------------------------\n";
		close(new_socket);
	}
    return 0;
}
