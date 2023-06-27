/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:41:36 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/17 17:13:52 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

HttpResponse::HttpResponse() {
	// _it = _headers.begin();
}

void HttpResponse::setVersion(std::string const &version) {
	_version = version;
}

void HttpResponse::setStatusCode(int status_code) {
	_status_code = status_code;
}

void HttpResponse::setStatusMessage(std::string const &status_message) {
	_status_message = status_message;
}

void HttpResponse::setHeader(const std::string &name, const std::string &value) {
	// std::cout << name << ": " << value << std::endl;
	_headers[name] = value;
	// _headers.insert(_it, std::pair<std::string, std::string>(name, value));
}

void HttpResponse::setBody(const std::string &body) {
	_body = body;
}

std::string HttpResponse::generateStatusLine() {
	return (_version + " " + std::to_string(_status_code) + " " + _status_message);
}

// !!!!!!!!!!!!!!   take in mainde when implement this function that the map is revirse order
// generate headers by contatinet it in one string that contian  (KEY: value\nKEY: value\n......\n\n)
std::string HttpResponse::generateHeaders() {
	std::string headers;

	std::map<std::string, std::string>::iterator it = _headers.end();
	it--;
	for (; it != _headers.begin(); it--)
	{
		headers += it->first;
		headers += ": ";
		headers += it->second;
		headers += "\r\n";
	}
		headers += it->first;
		headers += ": ";
		headers += it->second;
		headers += "\r\n";
	headers += "\r\n";

	return (headers);
}

unsigned int HttpResponse::getBodySize() const {
	return (_body.size());
}

std::string HttpResponse::generateResponse() {
	
	std::string response;
	// std::cout << generateStatusLine() << std::endl;
	response = 	generateStatusLine() + "\r\n" + generateHeaders() + _body;

	return (response);
}

HttpResponse::~HttpResponse() {

	// Just to dubag 
	// std::cout << "-----Start------\n";
	// std::map<std::string, std::string>::iterator it = _headers.begin();
	// for (; it != _headers.end(); it++)
	// {
	// 	std::cout << it->first << ": " << it->second	<< std::endl;
	// }
	// std::cout << "-----End------\n";
}