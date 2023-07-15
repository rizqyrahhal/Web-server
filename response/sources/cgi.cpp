/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 03:33:15 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/15 02:40:46 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cgi.hpp"
#include "../includes/responsePart.hpp"
#include <cstring>
#include <signal.h>

Cgi::Cgi() {
	
}

char* strdup98(const char* str) {
    if (str == NULL)
        return NULL;
    size_t length = strlen(str);
    char* duplicate = new char[length + 1]; // Allocate memory for the duplicate string
    strcpy(duplicate, str); // Copy the string
    return duplicate;
}

std::string searchInRequestedHeader(const std::map<std::string, std::string>& Map, const std::string& key) {
    std::map<std::string, std::string>::const_iterator it = Map.find(key);
    if (it != Map.end()) {
        return it->second;
    }
    return "";  // Return an empty string if the key is not found
}

std::string Cgi::getCgiPath(std::map<std::string, std::string> map, std::string _contentType) {
	std::string cgi_bin;
    if (!map.empty()) {
		
        if (_contentType == "application/x-httpd-php")
            cgi_bin = map["php"];
        else if (_contentType == "application/x-python-code"){
			
            cgi_bin = map["by"];
    	// std::cout << map["by"] << " ------ ------ ------ ------ ------ ------\n";    //// !!!! here when stop bucouse cgi need more parse
		}
    }
    if (cgi_bin.empty())		
        throw(500);
	return (cgi_bin);
}

char** Cgi::vectorToCharArray(std::vector<const char*> vec) {
    char** arr = new char*[vec.size()];
    for (size_t i = 0; i < vec.size(); ++i) {
        arr[i] = const_cast<char *>(vec[i]);
    }
    return arr;
}

void Cgi::fillArgv(std::string &cgibinpath, std::string &_requestedSource) {
	_argv.push_back(strdup98(cgibinpath.c_str()));
	_argv.push_back(strdup98(_requestedSource.c_str()));
	_argv.push_back(NULL);
}

void Cgi::fillEnvp(request request, server server, std::string requstedsource, std::string contenttype) {
	_envp.push_back(strdup98(std::string("SERVER_PROTOCOL=HTTP/1.1").c_str()));
	_envp.push_back(strdup98(std::string("REDIRECT_STATUS=200").c_str()));
	_envp.push_back(strdup98(std::string("SERVER_SOFTWARE=HTTP/1.1").c_str()));
	_envp.push_back(strdup98(std::string("REQUEST_METHOD=" + request.method).c_str()));
	_envp.push_back(strdup98(std::string("QUERY_STRING=" + request.query).c_str()));
	_envp.push_back(strdup98(std::string("PATH_INFO=" + requstedsource).c_str()));
	_envp.push_back(strdup98(std::string("HTTP_COOKIE=" + searchInRequestedHeader(request.map_request, "Cookie")).c_str()));
	if (!searchInRequestedHeader(request.map_request, "Content-Length").empty()){
		_envp.push_back(strdup98(std::string("CONTENT_LENGTH=" + searchInRequestedHeader(request.map_request, "Content-Length")).c_str()));
	}
	else if(!requstedsource.empty()){
		_envp.push_back(strdup98(std::string("CONTENT_LENGTH=" + std::to_string(calculeBodySize(requstedsource))).c_str()));
	}
	else
		_envp.push_back(strdup98(std::string("CONTENT_LENGTH=").c_str()));
	if (!searchInRequestedHeader(request.map_request, "Content-Type").empty())
		_envp.push_back(strdup98(std::string("CONTENT_TYPE=" + searchInRequestedHeader(request.map_request, "Content-Type")).c_str()));
	else if(!contenttype.empty()) 
		_envp.push_back(strdup98(std::string("CONTENT_TYPE=" + contenttype).c_str()));
	else
		_envp.push_back(strdup98(std::string("CONTENT_TYPE=").c_str()));
	_envp.push_back(strdup98(std::string("GATEWAY_INTERFACE=CGI/1.1").c_str()));
	_envp.push_back(strdup98(std::string("REMOTE_ADDR=" + server.ip_address).c_str()));
	_envp.push_back(strdup98(std::string("SCRIPT_NAME=" + requstedsource).c_str()));
	_envp.push_back(strdup98(std::string("SCRIPT_FILENAME=" + requstedsource).c_str()));
	_envp.push_back(strdup98(std::string("SERVER_NAME=" + server.server_name).c_str()));
	_envp.push_back(strdup98(std::string("REMOTE_HOST=" + searchInRequestedHeader(request.map_request, "Host")).c_str()));
	_envp.push_back(strdup98(std::string("SERVER_PORT=").c_str()));
	_envp.push_back(NULL);


	
	// std::cout << "Vector[0]: " << _envp[0] << std::endl;
	// std::cout << "Vector[0]: " << _envp[1] << std::endl;
	// std::cout << "Vector[0]: " << _envp[2] << std::endl;
	// std::cout << "Vector[0]: " << _envp[3] << std::endl;
	// std::cout << "Vector[0]: " << _envp[4] << std::endl;
	// std::cout << "Vector[0]: " << _envp[5] << std::endl;
	// std::cout << "Vector[0]: " << _envp[6] << std::endl;
	// std::cout << "Vector[0]: " << _envp[7] << std::endl;
	// std::cout << "Vector[0]: " << _envp[8] << std::endl;
	// std::cout << "Vector[0]: " << _envp[9] << std::endl;
	// std::cout << "Vector[0]: " << _envp[10] << std::endl;
	// std::cout << "Vector[0]: " << _envp[11] << std::endl;
	// std::cout << "Vector[0]: " << _envp[12] << std::endl;
	// std::cout << "Vector[0]: " << _envp[13] << std::endl;
	// std::cout << "Vector[0]: " << _envp[14] << std::endl;
}

void Cgi::execut(std::string cgibin, char **argv, char **envp, std::string _requestedSource, int file) {
	// int file = open(_requestedSource.c_str(), 666);
	(void)_requestedSource;
	int fd[2];
	pipe(fd);
	pid_t pid = fork();
	if (pid == 0) {
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		dup2(file, 0);
		close(file);
		// execve(cgibin.c_str(), argv, envp);
		if (execve(cgibin.c_str(), argv, envp) == -1)
		{
			// perror("execve:");
			// std::cout << "--------------------iiiiiii--------------\n";
			throw(500);
		}
	}
	else {
		int status;
		sleep(1);
		pid_t  wait_reat = waitpid(pid, &status, WNOHANG);
		if (wait_reat == 0) {
			kill(pid, SIGKILL);
			throw(500);
		}
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		std::vector<char> buffer(2606); // calculate this size before all  !!!!!!!!!!!!!!!!!!!! this is Just hard code  (use getline() or some thing like this read pares andd assinge in string body)
		int size = read(0, &buffer[0], 2606);
	(void)size;
        #ifdef DEBUG
		std::cout << "************** Response Genarated by CGI **************\n Genarate this SIZE: " << size  \
					<< "\n" << std::string(buffer.begin(), buffer.end()) << std::endl;
        #endif

		throw(std::string(buffer.begin(), buffer.end()));
	}
}

Cgi::~Cgi() {

	// this too function be to delete memory leak if exist after check
	_envp.clear();
	_argv.clear();
}










