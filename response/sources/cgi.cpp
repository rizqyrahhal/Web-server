/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 03:33:15 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/14 08:16:14 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cgi.hpp"
#include "../includes/responsePart.hpp"
#include <cstring>


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

Cgi::Cgi() {
	
}
Cgi::~Cgi() {
	
}

char* strdup98(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    size_t length = strlen(str);
    char* duplicate = new char[length + 1]; // Allocate memory for the duplicate string

    strcpy(duplicate, str); // Copy the string

    return duplicate;
}

void Cgi::fillEnvp(std::map<std::string, std::string> &map_request, std::string method, std::string query, std::string requstedsource) {
	
	_envp.push_back(strdup98(std::string("REQUEST_METHOD=" + method).c_str()));
	_envp.push_back(strdup98(std::string("QUERY_STRING=" + query).c_str()));
	_envp.push_back(strdup98(std::string("SERVER_PROTOCOL=HTTP/1.1").c_str()));
	_envp.push_back(strdup98(std::string("PATH_INFO=" + requstedsource).c_str()));
	// if (!searchInRequestedHeader("CONTENT_LENGTH").empty())
	// 	_envp.push_back(strdup98(std::string("CONTENT_LENGTH=" + searchInRequestedHeader("CONTENT_LENGTH")).c_str()));
	// else
	// 	_envp.push_back(strdup98(std::string("CONTENT_LENGTH=" + getFileBodySize(requstedsource)).c_str()));

	// _envp.push_back(strdup98(std::string("CONTENT_TYPE=" + searchInRequestedHeader("CONTENT_TYPE")).c_str()));
	_envp.push_back(strdup98(std::string("GATEWAY_INTERFACE=").c_str()));
	// _envp.push_back(strdup98(std::string("REMOTE_ADDR=" + getServerIp()).c_str()));
	_envp.push_back(strdup98(std::string("SCRIPT_NAME=").c_str()));
	_envp.push_back(strdup98(std::string("SERVER_NAME=").c_str()));
	_envp.push_back(strdup98(std::string("REMOTE_HOST=").c_str()));
	_envp.push_back(strdup98(std::string("SERVER_PORT=").c_str()));
	_envp.push_back(strdup98(std::string("SERVER_SOFTWARE=").c_str()));
	_envp.push_back(strdup98(std::string("REDIRECT_STATUS=").c_str()));
	_envp.push_back(strdup98(std::string("Content-Length=").c_str()));	

	std::cout << "Vector[0]: " << _envp[0] << std::endl;
	std::cout << "Vector[0]: " << _envp[1] << std::endl;
	std::cout << "Vector[0]: " << _envp[2] << std::endl;
	std::cout << "Vector[0]: " << _envp[3] << std::endl;
	std::cout << "Vector[0]: " << _envp[4] << std::endl;
	std::cout << "Vector[0]: " << _envp[5] << std::endl;
	std::cout << "Vector[0]: " << _envp[6] << std::endl;
	std::cout << "Vector[0]: " << _envp[7] << std::endl;
	std::cout << "Vector[0]: " << _envp[8] << std::endl;
	std::cout << "Vector[0]: " << _envp[9] << std::endl;
	std::cout << "Vector[0]: " << _envp[10] << std::endl;
	std::cout << "Vector[0]: " << _envp[11] << std::endl;

	(void)map_request;
	(void)method;
	(void)query;
}

void Cgi::execut(std::string cgibin, char **argv, char **envp, std::string _requestedSource) {
	// (void)envp;
	int file = open(_requestedSource.c_str(), 666);
	int fd[2];
	pipe(fd);
	pid_t pid = fork();
	if (pid == 0) {
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		dup2(file, 0);
		close(file);
		// execve("./tools/cgi-bin/php-cgi.exe", argv, envp);
		execve(cgibin.c_str(), argv, envp);
		perror("execve");
		exit(-1);
	}
	else {
	int status;
	waitpid(pid, &status, 0);
	close(fd[1]);
	dup2(fd[0], 0);
	close(fd[0]);
	std::vector<char> buffer(2606); // calculate this size bfore all
	int size = read(0, &buffer[0], 2606);
	std::cout << "************** Response Genarated by CGI **************\n Genarate this SIZE: " << size  << "\n" << std::string(buffer.begin(), buffer.end()) << std::endl;
	
	std::string str(buffer.begin(), buffer.end());
	throw(std::string("HTTP/1.1 200 Ok\r\n" + str)); // harde code (the start line exactly the status code  be to make compatible with the cgi status header)
	// throw(std::string(buffer.begin(), buffer.end()));
	}
}

// conctenet_envp_in_char2pointer();  // my be pass &vector[0]  if work