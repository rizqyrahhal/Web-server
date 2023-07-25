/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 03:15:57 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/25 01:17:45 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "responsePart.hpp"

class request;
class server;

class Cgi
{
    private:
		std::string cgiBinPath;
		std::string cgiOutput;
	private:
	public:
		std::vector<const char *> _envp;
		std::vector<const char *> _argv;
		Cgi();
		std::string getCgiPath(std::map<std::string, std::string> map, std::string _contentType);

		char** vectorToCharArray(std::vector<const char*> vec);
	    void fillArgv(std::string &cgibinpath, std::string &_requestedSource);
		void fillEnvp(request request, server server, std::string requstedsource, std::string contenttype);
		std::string &execut(std::string cgibin, char **argv, char **envp, int file);
		~Cgi();
};