/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetMethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:24:42 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/11 03:14:07 by rarahhal         ###   ########.fr       */
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

	if (_resourceType == DRCT) {
        checkForIndexFile(&(*this), server);
    }
    Response::GetContentType(_requestedSource, _mimeTypes, _contentType);
	
	if (_contentType.empty())  // here working on cgi files     !!!! (becous the FILS AND DIRECTORYS RELATION IN THE FILE REQUSTED ME BY CHECK THE DYROCTORY IF HAVENT IN INDEX FILE then work as file)
	{
		// 	metaVariablesName.push_back("REQUEST_METHOD");
		// 	metaVariablesName.push_back("QUERY_STRING");
		// 	metaVariablesName.push_back("CONTENT_LENGTH");
		// 	metaVariablesName.push_back("CONTENT_TYPE");
		// 	metaVariablesName.push_back("GATEWAY_INTERFACE");
		// 	metaVariablesName.push_back("PATH_INFO");
		// 	metaVariablesName.push_back("REMOTE_ADDR");
		// 	metaVariablesName.push_back("SCRIPT_NAME");
		// 	metaVariablesName.push_back("SERVER_NAME");
		// 	metaVariablesName.push_back("SERVER_PORT");
		// 	metaVariablesName.push_back("SERVER_PROTOCOL");
		// 	metaVariablesName.push_back("SERVER_SOFTWARE");

		// _metaVariables = new char*[13];
		// _metaVariables[0] = new char[15 + request.method.size()];
		// std::string tafrigh(std::string(metaVariablesName[0]) + "=" + request.method);
		// _metaVariables[0] = tafrigh.c_str();
		
		// _metaVariables[1] = new char[13 + request.query.size()];
		// std::string tafrigh2(std::string(metaVariablesName[1]) + "=" + request.query);
		// _metaVariables[0] = tafrigh2.c_str();


		size_t uy = request.map_request.size();
		std::cout << "~~~~~~~~~~~~~~~~~~~~ Request Headers ~~~~~~~~~~~~~~~~~~~~\nHEADER_SIZE: "<< uy << std::endl;
		for (std::map<std::string, std::string>::iterator it = request.map_request.begin(); it != request.map_request.end(); ++it) {
			std::cout << it->first << "=" << it->second << std::endl << uy << std::endl;
			uy--;
		}

		char* argv[] = {
        (char*)"./www/cgi/php-cgi.exe",
        (char*)_requestedSource.c_str(),
        nullptr
        };
		// char* envp[] = {
		// (char*)"REQUEST_METHOD=GET",
		// (char*)"REDIRECT_STATUS=0",
		// // (char*)"",
		// // (char*)"",
		// nullptr
		// };
		int fd[2];
		pipe(fd);
		pid_t pid = fork();
    	if (pid == 0) {
			dup2(fd[1], 1);
			close(fd[1]);
    		execve("./www/cgi/php-cgi.exe", argv, NULL);
    		perror("execve");
    		exit(-1);
   		}
    	else {
    	int status;
    	waitpid(pid, &status, 0);
		dup2(fd[0], 0);
		close(fd[0]);
		std::vector<char> buffer(2606);
		int size = read(0, &buffer[0], 2606);
		std::cout << "************** Response Genarated by CGI **************\n Genarate this SIZE: " << size  << "\n" << std::string(buffer.begin(), buffer.end()) << std::endl;
		
		std::string str(buffer.begin(), buffer.end());
			throw(std::string("HTTP/1.1 200 Ok\r\n" + str)); // harde code (the start line exactly the status code  be to make compatible with the cgi status header)
			// throw(std::string(buffer.begin(), buffer.end()));
    		}
		}

        setHeader("Content-Type", _contentType);
        #ifdef DEBUG
            std::cout << "contentType : " << _contentType << std::endl;
        #endif
        setBody(generatBody(_requestedSource));
        throw(200);
}





     /* autoindex template */
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








						/* the cgi testing work */

// #include <iostream>
// #include <string>
// #include <unistd.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>
		// std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
        // if (_requestedSource == "./tools/cgi-scripts/php/upload_simple.php")
		// {
		// 	char* argv[] = {
        //     (char*)"./www/cgi/php-cgi.exe",
        //     (char*)"./tools/cgi-scripts/php/upload_simple.php",
        //     nullptr
        // 	};
		// 	// char* envp[] = {
		// 	// (char*)"REQUEST_METHOD=GET",
		// 	// // (char*)"",
		// 	// // (char*)"",
		// 	// nullptr
		// 	// };
		// 	int fd[2];
		// 	pipe(fd);
		// 	pid_t pid = fork();
    	// 	if (pid == 0)
    	// 	{
		// 		dup2(fd[1], 1);
		// 		close(fd[1]);
        // 		execve("./www/cgi/php-cgi.exe", argv, NULL);
       	// 		perror("execve");
        // 		exit(-1);
   		// 	}
    	// else
    	// {
        // 	int status;
        // 	waitpid(pid, &status, 0);
		// 	dup2(fd[0], 0);
		// 	close(fd[0]);
		// 	std::vector<char> buffer(2606);
		// 	int size = read(0, &buffer[0], 2606);
		// 	std::cout << "************** Response Genarated by CGI **************\n Genarate this SIZE: " << size  << "\n" << std::string(buffer.begin(), buffer.end()) << std::endl;
			
		// 	std::string str(buffer.begin(), buffer.end());
		// 	throw(std::string("HTTP/1.1 200 Ok\r\n" + str));
		// 	// throw(std::string(buffer.begin(), buffer.end()));
    	// }
		// }
		// std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";