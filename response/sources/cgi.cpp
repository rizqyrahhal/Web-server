/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 03:33:15 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/13 03:42:38 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

void Response::cgi(server server, request request) {
	(void)server;
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

		std::cout << "****888888****: " << _requestedSource << std::endl;
		char* argv[] = {
        (char*)"./tools/cgi-bin/php-cgi",
        // (char*)"./cgi_tester",
        (char*)_requestedSource.c_str(),
        nullptr
        };
		char* envp[] = {
		(char*)"SERVER_PROTOCOL=HTTP/1.1",
		(char*)"REQUEST_METHOD=GET",
		(char*)"REDIRECT_STATUS=1",
		(char*)"PATH_INFO=./www/index.php",
        (char*)"SERVER_PORT=8080",
		(char*)"AUTH_TYPE",
		(char*)"CONTENT_LENGTH",
        (char*)"CONTENT_TYPE",
		(char*)"GATEWAY_INTERFACE",
		(char*)"PATH_TRANSLATED",
        (char*)"QUERY_STRING",
		(char*)"REMOTE_ADDR",
        (char*)"REMOTE_HOST",
		(char*)"REMOTE_IDENT",
        (char*)"REMOTE_USER",
        (char*)"SCRIPT_NAME",
		(char*)"SERVER_NAME",
        (char*)"SERVER_SOFTWARE",
		// (char*)"Accept-Encoding=gzip, deflate, br",
		// (char*)"Accept-Language=en-US,en;q=0.9",
		// (char*)"Cache-Control=max-age=0",
		// (char*)"Connection=keep-alive",
		// (char*)"Cookie=_ga=GA1.1.1276071924.1689035207; _ga_BVS1XM9LYJ=GS1.1.1689035207.1.1.1689035438.8.0.0; __gads=ID=d57167e481e2e578-223635ec3ae00050:T=1689035229:RT=1689036300:S=ALNI_MZeZpUb6CJn5WXJvh1UhjY-QILGAw; __gpi=UID=00000c65db6194a3:T=1689035229:RT=1689036300:S=ALNI_MZg5-InE8f8kHbBfHhC8yHKkIfFeg; PHPSESSID=a6dd5c948bdbfebf6dc11f830e45afb3",
		// (char*)"Host=localhost:8080",
		// (char*)"Sec-Fetch-Dest=document",
		// (char*)"Sec-Fetch-Mode=navigate",
		// (char*)"Sec-Fetch-Site=none",
		// (char*)"Sec-Fetch-User=?1",
		// (char*)"Upgrade-Insecure-Requests=1",
		// (char*)"User-Agent=Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36",
		// (char*)"sec-ch-ua='Not.A/Brand';v='8', 'Chromium';v='114', 'Google Chrome';v='114'",
		// (char*)"sec-ch-ua-mobile=?0",
		// (char*)"sec-ch-ua-platform='macOS'",
		// (char*)"Accept=text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
		// (char*)"",
		// (char*)"",
		nullptr
		};
		(void)envp;
			int fd[2];
			pipe(fd);
			pid_t pid = fork();
    		if (pid == 0) {
				close(fd[0]);
				dup2(fd[1], 1);
				close(fd[1]);
        		execve("./tools/cgi-bin/php-cgi", argv, envp);
        		// execve("./cgi_tester", argv, envp);
       			perror("execve");
        		exit(-1);
   			}
    		else {
        	int status;
        	waitpid(pid, &status, 0);
			close(fd[1]);
			dup2(fd[0], 0);
			close(fd[0]);
			std::vector<char> buffer(2606);
			int size = read(0, &buffer[0], 2606);
			std::cout << "************** Response Genarated by CGI **************\n Genarate this SIZE: " << size  << "\n" << std::string(buffer.begin(), buffer.end()) << std::endl;
			
			std::string str(buffer.begin(), buffer.end());
			isCgi = true;
			throw(std::string("HTTP/1.1 200 Ok\r\n" + str)); // harde code (the start line exactly the status code  be to make compatible with the cgi status header)
			// throw(std::string(buffer.begin(), buffer.end()));
    		}
}