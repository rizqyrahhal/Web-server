/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server-side.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 17:33:12 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/18 18:10:13 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "switch-data.hpp"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include "../includes/responsePart.hpp"

#define PORT 4875

void network(void)
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

    Request request;   //response
	
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


		// ############################################  INSIDE MY PART   ###################################//
		// ##################################################################################################//
		// ##################################################################################################//
		
		// std::cout << "00000000000000000\n"; 
		// std::string response = CreatResponse(); // it is a probleme when send response in one message, with body just contian file html 
		// std::cout << "----SIZE: " << response.size() << std::endl;

		// ---------------------  detecte status code and fill all needed data to generate response

		// ++++++++++++++++++++++++++++  body handler
        // std::ifstream file("www/app/index.html", std::ifstream::binary);
        // if (!file.is_open())
        // {
        // 	std::cout << "Error in opening file\n";
        // 	std::exit(-404);
        // }
        // file.seekg(0, std::ios::end);
        // int Length = file.tellg();
        // file.seekg(0, std::ios::beg);

        // char uffer[(int)Length/2 +1];
		// char uffer2[(int)Length/2 + 2];
        // file.read(uffer, (int)Length/2);
        // file.read(uffer2, (int)Length/2 + 1);
		// // uffer[(int)Length/2 + 1] = '\0';
		// // uffer2[(int)Length/2 + 2] = '\0';

        // file.close();
        // // ############################


		// // **********************


        // // --------------- generate response
        // HttpResponse new_response;

        // new_response.setVersion(request.getVersion());
        // new_response.setStatusCode(200);
        // new_response.setStatusMessage("Ok"); // generat enume structe to geting statuse msg from its
		// // new_response.setHeader("Date: ", generateDate());
        // new_response.setHeader("Content-Type", "text/html"); //generateContentType()
        // new_response.setHeader("Content-Length", std::to_string(Length));
		// new_response.setHeader("Conection", request.getConection());
        // // new_response.setBody(uffer);
		// std::string response = new_response.generateResponse();
        // std::cout << "\n\n\n" << response << "\n\n\n";
	    // // ******************
		
        std::string response = GenerateResponseFromStatusCode(400);
        std::cout << "\n\n\n" << response << "\n\n\n";




		// ##################################################################################################//
		// ##################################################################################################//
		// ##################################################################################################//
        // ------------------ sending response 
        if(request.getMethod() == "GET" && request.getUri() == "/index.html" && request.getVersion() == "HTTP/1.1")
        {
            if (send(new_socket, response.c_str(), response.size(), MSG_SEND) < -1)
				std::cout << "NETWORK!!!!!!!!!!!!!!!!!!\n";
            // send(new_socket, uffer, Length/2, MSG_SEND);
            // send(new_socket, uffer2, Length/2 + 1, MSG_SEND);
        }
        // *******************
	}
}
	    // char uffer2[(int)videoLength/2 + 1];
	    // file.read(uffer2, (int)videoLength/2 + 1);

		// send(new_socket, header.c_str(), header.size(), MSG_SEND);
		// send(new_socket, uffer, (int)videoLength/2, MSG_SEND);
		// send(new_socket, uffer2, (int)videoLength/2 + 1, MSG_SEND);
		// // write(new_socket, headers.c_str(), response.size());
		// // write(new_socket, uffer, 3465);
		// std::cout << "-------------------response message sent---------------\n";
		// std::cout << response << std::endl;
		// std::cout << "------------------------------------------------------\n";
		// close(new_socket);
