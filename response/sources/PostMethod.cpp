/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 16:45:33 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/11 03:17:16 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"
bool supportUpload(locations matchedlocation) {
    if (!matchedlocation.upload_pass.empty())
        return true;
    return false;
}

void Response::PostMethod(server server, request request) {
    std::cout << "Upload_pass: " << server.locations[_matchedLocationPosition].upload_pass\
              << std::endl << "Requested Method: " << request.method << std::endl;
    
    /* in this condition setup the position to upload the body of request */
    if(supportUpload(server.locations[_matchedLocationPosition])) {
        // getFileName(); genarate or detect the filename

		/* here trow some number to return after it the nececery information to upload the body the file where upload it */
        throw(201);  // Just hardcode be to check if uploaded
    }
    /* here the other way, working on location dosn't support upload */
    else {
        _requestedSource = Response::GetRequestedSource(server.locations[Response::_matchedLocationPosition], request.url, _resourceType, &(*this), request.method);

		#ifdef DEBUG
        std::cout << "_requestedSource: " << _requestedSource << std::endl;
        std::cout << "_resourceType: ";
        if(_resourceType)
            std::cout << "FILE\n";
        else
            std::cout << "DERCTORY\n";
        #endif

        if (_resourceType == DRCT && !checkIndexInsidDerctory(&_requestedSource)) {
			throw(403);
        }
		else if (server.locations[_matchedLocationPosition].cgi.empty())
			throw(403);
		else {
			/* run cgi on requested file with POST request_method
			Return code Depend on CGI */
		/* here trow some number to return after it the nececery information to run cgi with requsted file */


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
    }
}
