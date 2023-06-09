/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server-side.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 17:33:12 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/06 17:48:16 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

// read
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

// open
#include <fcntl.h>

#define PORT 8080

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
	
	if (bind(server_fd, (struct sockaddr *)&adderss, sizeof(adderss)) < 0)
		std::cout << "ERROR in binding socket\n";
	
	if (listen(server_fd, 10) < 0)
		std::cout << "ERROR in listen\n";

	long valread;

	int file = open("index.html", O_RDONLY, 666);
	if (file < 0)
		std::cout << "Error in opning file\n";
	char buffer[1563];
	int size =  read(file, buffer, 1563);
	if (size < 0)
		std::cout << "Error in raeding file\n";
	close(file);
	std::string headers = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 1563\n\n";
	std::string response = headers + buffer;
	// hello.append(buffer);
	
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
		write(new_socket, response.c_str(), response.size());
		std::cout << "-------------------response message sent---------------\n";
		std::cout << response << std::endl;
		std::cout << "------------------------------------------------------\n";
		close(new_socket);
	}
    return 0;
}