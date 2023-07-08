/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetMethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:24:42 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/08 01:04:54 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

std::string generateAutoindexFile(std::string requestedSource) {
    
    std::string indexhtml = "<!-- DOCTYPE html --><html><head><title>Directory Listing</title></head><body><h1>Directory Listing</h1><ul>";
    DIR* dr = opendir(requestedSource.c_str());
    
    if (dr != NULL)
    {
        struct dirent *dir = NULL;
        while ((dir = readdir(dr)))
            indexhtml += "<li><a href=\"" + std::string(dir->d_name) + "\">" + dir->d_name + "</a></li>";
        indexhtml += "</ul></body></html>";
        closedir(dr);
    }
    else
        std::cout << "ERROR IN OPENING DIRECTORY (this error be to check about it in parsing!!!)";
    return (indexhtml);
}


#include <iostream>
#include <string>
#include <unistd.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


/* this function for new return a response inside string,
    but for advance return object that contian header indide string,
    and file of body and member function to red from this file by spesific size */
void Response::GetMethod(server server, request request) {
    _requestedSource = Response::GetRequestedSource(server.locations[Response::_matchedLocationPosition], request.url, _resourceType, &(*this), request.method);
    #ifdef DEBUG
        std::cout << "_requestedSource: " << _requestedSource << std::endl;
        std::cout << "_resourceType: ";
        if(_resourceType)
            std::cout << "FILE\n";
        else
            std::cout << "DERCTORY\n";
    #endif
	
    if (_resourceType == DRCT || _resourceType == FILE)
    {
		
		std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
        if (_requestedSource == "./www/cgi/test.php")
		{
			char* argv[] = {
            (char*)"./www/cgi/php-cgi.exe",
            (char*)"-f",
            (char*)"./www/cgi/test.php",
            nullptr
        	};
			int fd[2];
			pipe(fd);
			pid_t pid = fork();
    		if (pid == 0)
    		{
				dup2(fd[1], 1);
				close(fd[1]);
        		execve("./www/cgi/php-cgi.exe", argv, NULL);
       			perror("execve");
        		exit(-1);
   			}
    	else
    	{
        	int status;
        	waitpid(pid, &status, 0);
			dup2(fd[0], 0);
			close(fd[0]);
			std::vector<char> buffer(5);
			int size = read(0, &buffer[0], 5);
			std::cout << "************** Response Genarated by CGI **************\n Genarate this SIZE: " << size  << "\n" << std::string(buffer.begin(), buffer.end()) << std::endl;
			throw(std::string(buffer.begin(), buffer.end()));
    	}
		}
		std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
        
		if (_resourceType == DRCT) {
            checkForIndexFile(&(*this), server);
        }
        Response::GetContentType(_requestedSource, _mimeTypes, _contentType);
        setHeader("Content-Type", _contentType);
        #ifdef DEBUG
            std::cout << "contentType : " << _contentType << std::endl;
        #endif
        setBody(generatBody(_requestedSource));
        throw(200);
    }
    else if (_resourceType != DRCT && _resourceType != FILE) {

		/* CGI test work */
        if (_requestedSource == "index.php")
		{
			char* argv[] = {
            (char*)"./www/cgi/php-cgi.exe",
            (char*)"-f",
            (char*)"./www/cgi/test.php",
            nullptr
        	};
			int fd = 0;
			pid_t pid = fork();
			dup2(1, fd);
    		if (pid == 0)
    		{
        		execve("./www/cgi/php-cgi.exe", argv, NULL);
       			perror("execve");
        		exit(-1);
   			}
    	else
    	{
        	int status;
        	waitpid(pid, &status, 0);
			std::vector<char> buffer(1024);
			int size = read(fd, &buffer[0], 1024);
			std::cout << "************** Response Genarated by CGI **************\n Genarate this SIZE: " << size  << "\n" << std::string(buffer.begin(), buffer.end()) << std::endl;
    	}
		}
                    //cgi
        /* my be function to excute cgi if file or directory have cgi */
    }
}

// <!-- DOCTYPE html -->
// <html>
// <head><title>Directory Listing</title></head>
// <body>
// <h1>Directory Listing</h1>
// <ul>
// <li><a href="./www/app/html/.">.</a></li>
// <li><a href="./www/app/html/..">..</a></li>
// <li><a href="./www/app/html/images">images</a></li>
// <li><a href="./www/app/html/logo.png">logo.png</a></li>
// <li><a href="./www/app/html/Music.mp4">Music.mp4</a></li>
// <li><a href="./www/app/html/scripts">scripts</a></li>
// <li><a href="./www/app/html/styles">styles</a></li>
// <li><a href="./www/app/html/videoplayback.mp4">videoplayback.mp4</a></li>
// </ul>
// </body>
// </html>