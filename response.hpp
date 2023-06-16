/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 21:47:25 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/16 22:27:34 by rarahhal         ###   ########.fr       */
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
		void setBody(const std::string &body);
		std::string generateResponse();
		std::string generateResponse(int statuscode);

		~Response();
};





							/*   data     */
//  the Request class
class Request
{
    private:
		std::string _method;
		std::string _uri;
		std::string _version;
        std::string _conection;
		std::map<std::string, std::string> _headers;
		std::string _body;
    public:
		Request(){
            _method = "GET";
            _uri = "/index.html";
            _version = "HTTP/1.1";
            _conection = "close";
			_headers[""] = "";
        }
        std::string getMethod() const {
            return _method;
        }
        std::string getUri() const {
            return _uri;
        }
        std::string getVersion() const
        {
            return _version;
        }
        std::string getConection() const {
            return _conection;
        }
		~Request(){}
};

// the Location class
class Locations
{
	public:
		std::string _name;
		std::string _root;
		std::vector<std::string> _allow_methods;
		std::string _autoindex;
		std::map<std::string, std::string> _cgi;
		std::map<int, std::string> _redirect;
	public:
		Locations() {
			_name = "localhost";
			_root = "www/";
			_allow_methods.push_back("GET");
			_allow_methods.push_back("POST");
			_allow_methods.push_back("DELETE");
			_autoindex = "on";
			// _cgi ;
			// _redirect ;
		}
		~Locations(){}
};

// the Server class
class Server
{
    public:
	    int _port;
	    std::string _ip_address;
	    std::string _server_name;
	    std::vector<Locations> _locations;
	    std::map<int, std::string> _map_err_page;
	    int _client_body_size;
	public:
	    Server(){
            _port = 0000;
            _ip_address = "127.0.0.1";
			_server_name = "localhost";
			// _locations ;
			// _map_err_page ;
			// _client_body_size ;
        }
	    ~Server(){}
};