/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePart.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 15:35:12 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/23 21:05:13 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iterator>
#include <unistd.h>
#include <dirent.h>

#include "HttpResponse.hpp"
#include "statuscode.hpp"

#ifndef DEBUG
#define DEBUG true
#endif
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
            _uri = "/app/html/index.html";
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
		std::string _index; // if autoindex off         //this line not handl in parse talk with peer to handl it
		std::map<std::string, std::string> _cgi;
		std::map<int, std::string> _redirect;
	public:
		Locations(std::string name, std::string root) {
			_name = name;
			_root = root;
			_allow_methods.push_back("GET");
			_allow_methods.push_back("POST");
			_allow_methods.push_back("DELETE");
			_autoindex = "on";
			_index = "index.html";
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
			Locations location("/www/app", "/www/app/html");
			Locations location2("/example_redirect", "/www/app");
			location2._redirect[301] = "http://example.com";
			Locations location3("/www/app", "/www/app");
			Locations location4("/www/app/images/", "/www/app");
			Locations location5("/app/images/", "/www/app");
			Locations location6("/", "/www/app/html");
            _port = 0000;
            _ip_address = "127.0.0.1";
			_server_name = "localhost";

            _map_err_page[400] = "./www/error/400.html";
            _map_err_page[409] = "./www/error/409.html";
			_locations.push_back(location);
			_locations.push_back(location2);
			_locations.push_back(location3);
			_locations.push_back(location4);
			_locations.push_back(location5);
			_locations.push_back(location6);
			// _map_err_page ;
			// _client_body_size ;
        }
	    ~Server(){}
};
// -----------------------------------------------------------------------------------------------------------------------------------------------

// class switch
// {
// 	private:
// 		std::string header;
// 		int fd;
// 	public:
// 		std::string readforsend(short sizetoread);
// };


class Response
{
	private:
	    static size_t _matchedLocationPosition;
		std::string _matchedLocation;
		std::string _requestedSource;
		std::string _resourceType; // dyrictore Vs file
		std::string content_type; // if it is file (.html, .css, .js, .png, .mp4 ..) my be fill it in map and construct it at constructer
		// std::string _path;
		// std::string _method;

		std::string GetMatchedLocationRequestUrl(std::vector<Locations> locations, std::string requesturi);
		static void IsLocationHaveRedirection(Locations matchedlocation);
		static void IsMethodAllowedInLocation(std::vector<std::string> allowedmethod, std::string requestmethod);
		std::string GetMethod(Server server, Request request);
		// static void PostMethod();
		// static void DeleteMethod();
	public:
		Response();
		std::string CreatResponse();

		// seters
		// geters
		~Response();
	private:
		// utilse
		static std::string GetResourceType(std::string requestedSource);
		static std::string GetRequestedSource(Locations matchedlocation, std::string requesturi);
};

std::string GenerateResponseFromStatusCode(int statuscode);
std::string GenerateResponseFromStatusCode(int statuscode, HttpResponse response);



// class Glb
// {
// 	private:
// 	public:
// 		Server server;
// 		Request request;
// };