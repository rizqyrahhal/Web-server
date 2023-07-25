/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 03:33:15 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/25 01:54:21 by rarahhal         ###   ########.fr       */
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
    char* duplicate = new char[length + 1];
    strcpy(duplicate, str);
    return duplicate;
}

std::string searchInRequestedHeader(const std::map<std::string, std::string>& Map, const std::string& key) {
    std::map<std::string, std::string>::const_iterator it = Map.find(key);
    if (it != Map.end()) {
        return it->second;
    }
    return "";  /* Return an empty string if the key is not found */
}

std::string Cgi::getCgiPath(std::map<std::string, std::string> cgi_map, std::string _contentType) {
	std::string cgi_bin;
	std::string extantion;
	std::map<std::string, std::string> mime_map;

	fillMimeTypes(mime_map);
    if (!cgi_map.empty()) {
		for(std::map<std::string, std::string>::iterator it = mime_map.begin(); it != mime_map.end(); it++) {
			if (it->second == _contentType) {
				extantion = it->first;
				extantion.erase(0, 1);
			}
		}
		if (!extantion.empty()) {	
			for(std::map<std::string, std::string>::iterator it = cgi_map.begin(); it != cgi_map.end(); it++) {
				if (it->first == extantion) {
					cgi_bin = it->second;
					break;
				}
			}
		}
    }
    if (cgi_bin.empty()) {
        throw(500);
	}
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
	_envp.push_back(strdup98(std::string("PATH_INFO=/Users/rarahhal/Desktop/mankhdmoch7na/tools/cgi-scripts/python/upload.py").c_str()));
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
	_envp.push_back(strdup98(std::string("SERVER_NAME=" + server._name).c_str()));
	_envp.push_back(strdup98(std::string("REMOTE_HOST=" + searchInRequestedHeader(request.map_request, "Host")).c_str()));
	_envp.push_back(strdup98(std::string("SERVER_PORT=").c_str()));
	_envp.push_back(NULL);
}

std::string &Cgi::execut(std::string cgibin, char **argv, char **envp, int file) {
	int fd[2];
	pipe(fd);
	pid_t pid = fork();
	if (pid == 0) {
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		dup2(file, 0);
		close(file);

		if (execve(cgibin.c_str(), argv, envp) == -1)
		{
			throw(502);
		}
	}
	else {
		int status;
		sleep(2);
		pid_t  wait_reat = waitpid(pid, &status, WNOHANG);
		if (wait_reat == 0) {
			kill(pid, SIGKILL);
			throw(502);
		}
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);

		char buffer[1025];
		int size_read;
		while((size_read = read(0, buffer, 1024)) > 0) {
            buffer[size_read] = '\0';
			cgiOutput += std::string(buffer);
        }
	}
	return (cgiOutput);
}

Cgi::~Cgi() {
    for (size_t i = 0; i < _envp.size(); ++i) {
        delete _envp[i];
    }

    for (size_t i = 0; i < _argv.size(); ++i) {
        delete _argv[i];
    }

	_envp.clear();
	_argv.clear();
}










