/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 21:47:25 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/15 16:39:39 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>

class Response
{
    private:
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
		void setStatusMessage(std::string const &status_message);
		void setStatusCode(int status_code);
		void setHeader(std::string &name, std::string const &value);
		std::string generateResponse();
		~Response();
};