/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 21:47:25 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/17 16:38:35 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "responsePart.hpp"

#ifndef HTTP_RESONSE_DEBUG
// #define HTTP_RESONSE_DEBUG
#endif

class HttpResponse
{
	protected:
		std::string _version;
		int _status_code;
		std::string _status_message;
		std::map<std::string, std::string> _headers;
		std::string _body;
		std::string generateStatusLine();
		std::string generateHeaders();
	public:
		HttpResponse();
		void setVersion(std::string const &version);
		void setStatusCode(int status_code);
		void setStatusMessage(std::string const &status_message);
		void setHeader(const std::string &name, const std::string &value);
		void setBody(const std::string &body);
		unsigned int getBodySize() const;
		std::string generateResponse();
		~HttpResponse();
};
