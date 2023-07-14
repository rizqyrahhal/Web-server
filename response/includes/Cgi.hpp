/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 03:15:57 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/14 19:27:17 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../includes/responsePart.hpp"

class request;
class server;

class Cgi
{
    private:
		std::string cgiBinPath;
	private:
		// searchInRequestedHeader(std::string envpName);
	public:
		std::vector<const char *> _envp;
		std::vector<const char *> _argv;
		Cgi();
		std::string getCgiPath(std::map<std::string, std::string> map, std::string _contentType);

		char** vectorToCharArray(std::vector<const char*> vec);
	    void fillArgv(std::string &cgibinpath, std::string &_requestedSource);
		void fillEnvp(request request, server server, std::string requstedsource);
		void execut(std::string cgibin, char **argv, char **envp, std::string _requestedSource);
		~Cgi();
};




















		//  {
		// 	size_t uy = map_request.size();
		// 	int i = 0;
		// 	std::cout << "~~~~~~~~~~~~~~~~~~~~ Request Headers ~~~~~~~~~~~~~~~~~~~~\nHEADER_SIZE: "<< uy << std::endl;
		// 	for (std::map<std::string, std::string>::iterator it = map_request.begin(); it != map_request.end(); ++it) {
		// 		std::string header = it->first + "=" + it->second;
				
		// 		// std::cout << "PpppppppPPppppppppPppppPPPPppppppPPPPp\n" << header << std::endl;
		// 		_envp.push_back(header.c_str());

		// 		// std::cout << it->first << "=" << it->second << std::endl << uy << std::endl;
		// 		uy--;
		// 		i++;
		// 	}

			
		// 	// for (std::vector<const char *>::iterator it = _envp.begin(); it != _envp.end(); ++it) {
		// 	// 	std::cout << *it<< std::endl << std::endl;
		// 	// }
		// }