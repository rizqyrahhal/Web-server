/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:41:36 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/16 12:22:09 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

Response::Response() {	
	// _it = _headers.begin();
}

void Response::setVersion(std::string const &version) {
	_version = version;
}

void Response::setStatusCode(int status_code) {
	_status_code = status_code;
}

void Response::setStatusMessage(std::string const &status_message) {
	_status_message = status_message;
}

void Response::setHeader(const std::string &name, const std::string &value) {
	// std::cout << name << ": " << value << std::endl;
	_headers[name] = value;
	// _headers.insert(_it, std::pair<std::string, std::string>(name, value));
}

void Response::setBody(std::string body) {
	_body = body;	
}

std::string Response::generateStatusLine() {
	return (_version + " " + std::to_string(_status_code) + " " + _status_message + "\n");
}

// !!!!!!!!!!!!!!   take in mainde when implement this function that the map is revirse order
// generate headers by contatinet it in one string that contian  (KEY: value\nKEY: value\n......\n\n)
std::string Response::generateHeaders() {
	std::string headers;

	std::map<std::string, std::string>::iterator it = _headers.end();
	it--;
	for (; it != _headers.begin(); it--)
	{
		headers += it->first;
		headers += ": ";
		headers += it->second;
		headers += "\n";
	}
	headers += "\n";
	
	return (headers);
}

std::string Response::generateResponse() {
	
	std::string response;
	// std::cout << generateStatusLine() << std::endl;
	response = 	generateStatusLine() + generateHeaders() + _body;

	return (response);
}

std::string Response::generateResponse(int statuscode) {
	
	std::string response;
	// std::cout << generateStatusLine() << std::endl;
	response = 	generateStatusLine() + generateHeaders() + _body;

	return (response);
}

Response::~Response() {

	// Just to dubag 
	// std::cout << "-----Start------\n";
	// std::map<std::string, std::string>::iterator it = _headers.begin();
	// for (; it != _headers.end(); it++)
	// {
	// 	std::cout << it->first << ": " << it->second	<< std::endl;
	// }
	// std::cout << "-----End------\n";
}
