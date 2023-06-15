/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 21:47:25 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/15 21:31:38 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iterator>

class Response
{
    private:
		// management data
		// std::map<std::string, std::string>::iterator _it; // using to increment .insert()

		// shared data
		std::string _version;
        int _status_code;
        std::string _status_message;
        std::map<std::string, std::string> _headers;
		std::string _body;
		std::string generateStatusLine();
		std::string generateHeaders();
    public:
		Response();
		void setVersion(std::string const &version);
		void setStatusCode(int status_code);
		void setStatusMessage(std::string const &status_message);
		void setHeader(const std::string &name, const std::string &value);
		void setBody(std::string body);
		std::string generateResponse();	
		~Response();
};