/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 11:09:55 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/11 03:32:40 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

bool writeAccess(std::string requestedSource) {
	if (access(requestedSource.c_str(), W_OK) == 0)
	    return (true);
    else
        return (false);
}

bool deleteDirectory(std::string requestedSource) {
    DIR* dr = opendir(requestedSource.c_str());
    if (dr != NULL)
    {
        struct dirent *dir = NULL;
        while ((dir = readdir(dr))) {
			std::string name(dir->d_name);
			if (name == "." || name == "..")
				continue;
			if (opendir((requestedSource + name + "/").c_str()) != NULL)
				if (!deleteDirectory(std::string(requestedSource + name + "/")))
					return (false);
			if (std::remove((requestedSource + name).c_str()) == 0)
				continue;
			else
			{
        		closedir(dr);
                if (!writeAccess(requestedSource))
					throw(304);
				else
					throw(500);
			}
		}
        closedir(dr);
    }
    else {
        if (!writeAccess(requestedSource))
			throw(304);
		else
			throw(500);
    }
    return (true);
}

void Response::DeleteMethod(server server, request request) {
    _requestedSource = Response::GetRequestedSource(server.locations[Response::_matchedLocationPosition], request.url, _resourceType, &(*this), request.method);
    #ifdef DEBUG
        std::cout << "_requestedSource: " << _requestedSource << std::endl;
        std::cout << "_resourceType: ";
        if(_resourceType)
            std::cout << "FILE\n";
        else
            std::cout << "DERCTORY\n";
    #endif
    /* affter finish dyrectory error if no error and index take the same floow like file just set the neded information */
    if (_resourceType == DRCT || _resourceType == FILE) {  // change this condition in futere
		/* directory handling */
		if (_resourceType == DRCT) {
            if (!server.locations[_matchedLocationPosition].cgi.empty()) { /* CGI */
                if (!checkIndexInsidDerctory(&_requestedSource))  // here be to chenge to check index.php or index.by becouse its cgi
                    throw(403);
                /* run cgi on requested file with DELETE REQUEST_METHOD && then Return the code depending on CGI */ // doing that in cgi condition beloow
            }
			else if (deleteDirectory(_requestedSource)) // change it to else if in 11-07-2023
				throw(204);
        }

        /* CGI */
        if (server.locations[_matchedLocationPosition].cgi.empty()) {
            /* Return Code Depend on CGI */

            char* argv[] = {
            (char*)"./www/cgi/php-cgi.exe",
            (char*)_requestedSource.c_str(),
            nullptr
        	};

			char* envp[] = {
			(char*)"REQUEST_METHOD=DELETE",
			(char*)"REDIRECT_STATUS=0",
			// (char*)"",
			// (char*)"",
			nullptr
			};
			int fd[2];
			pipe(fd);
			pid_t pid = fork();
    		if (pid == 0) {
				dup2(fd[1], 1);
				close(fd[1]);
        		execve("./www/cgi/php-cgi.exe", argv, envp);
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

        /* file handling */
		if (std::remove(_requestedSource.c_str()) == 0)
			throw (204);
    }
}










/* the First implementation of this algo */
// bool deleteDirectory(std::string requestedSource) {
//     std::cout << "-------- ------------- :" << requestedSource << std::endl;
//     bool why = true;
//     DIR* dr = opendir(requestedSource.c_str());
//     if (dr != NULL)
//     {
//         struct dirent *dir = NULL;
//         while ((dir = readdir(dr))) {
// 			std::string name(dir->d_name);
// 			if (name == "." || name == "..")
// 				continue;
// 			if (opendir((requestedSource + name + "/").c_str()) != NULL)
// 			{
// 				if (!deleteDirectory(std::string(requestedSource + name + "/")))
// 					why = false;
// 			}
// 			if (std::remove((requestedSource + name + "/").c_str()) == 0)
// 				continue;
// 			else
// 			{
//         		// closedir(dr);
// 				why = false;
// 			}
// 		}
//         // closedir(dr);
//     }
//     else
//     {
//         std::cout << "ERROR IN OPENING DIRECTORY (this error be to check about it in parsing!!!)";
//         return false;
//     }
//     return (true);
// }