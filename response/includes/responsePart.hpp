/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePart.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 15:35:12 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/19 23:46:14 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <iterator>
#include <unistd.h>
#include <dirent.h>
#include <algorithm>


#include <sys/types.h>
#include <sys/wait.h>


#include "HttpResponse.hpp"
#include "statuscode.hpp"
#include "ResponseReturned.hpp"
#include "Cgi.hpp"
#include "../../networking/web_serv.hpp"

#define FILE true
#define DRCT false

#ifndef DEBUG
#define DEBUG
#endif

#ifndef GET_DEBUG
// #define GET_DEBUG
#endif

#ifndef POST_DEBUG
// #define POST_DEBUG
#endif

#ifndef DELETE_DEBUG
// #define DELETE_DEBUG
#endif

#ifndef UPLOAD_DEBUG
#define UPLOAD_DEBUG
#endif

#ifndef CGI_DEBUG
#define CGI_DEBUG
#endif



class request;
class server;
class locations;
class ResponseReturned;

class Response : public  HttpResponse
{
	private: /* data management */
		std::map<std::string, std::string> _mimeTypes;
		std::map<std::string, std::string> _cgi_mimeTypes;
		// std::vector<char*> metaVariablesName; 
		// char **_metaVariables;
	    static size_t _matchedLocationPosition;
		std::string _matchedLocation;
		std::string _requestedSource;
		bool _resourceType; /* dyrictore Vs file */
		std::string _contentType; // if it is file (.html, .css, .js, .png, .mp4 ..) my be fill it in map and construct it at constructer
		// std::string _path;
		std::string _method;

		/* Cgi*/
		std::string _cgiBinPath;
		// std::string _cgiOutput;

		/* functionalite */
		std::string ResponseGeneratedFromStatusCode(int statuscode, server server, request request, std::string &bodyfile, bool &isfile);
		std::string GetMatchedLocationRequestUrl(std::vector<locations> locations, std::string requesturi);
		static void IsLocationHaveRedirection(locations matchedlocation, Response &response);
		static void IsMethodAllowedInLocation(std::vector<std::string> allowedmethod, std::string requestmethod, Response &response);
		void GetMethod(server server, request request, std::string &bodyfile, bool &isfile);
		void DeleteMethod(server server, request request);
		void PostMethod(server server, request request);
	
		bool isCgi();
		void cgi(server server, request request, Response &response);
		void parseCgiOutput( const std::string& cgioutput);	

		/* response utilse */
		static void GetContentType(std::string requestedSource, std::map<std::string, std::string> mimetypes, std::string &contenttype);
		static std::string GetRequestedSource(locations matchedlocation, std::string requesturi, bool &resourcetype, Response *response, std::string method);
		static void checkForIndexFile(Response *response, server server, std::string &bodyfile, bool &isfile);
		
	public:
		Response();
		ResponseReturned CreatResponse(server server, request request);
		~Response();
	private: /* switched data */
		std::string bodyfile;
		bool isfile;
};

std::string GenerateResponseFromStatusCode(int statuscode);

/* mimetypes and content type function */
void fillMimeTypes(std::map<std::string, std::string> &mimeTypes);
std::map<std::string, std::string> readMimeTypes(const std::string& filePath);
std::string getFileExtantion(std::string requestedsource);
std::string getMimeType(std::map<std::string, std::string> mimetypes, std::string fileextantion);
std::string getExtantion(std::map<std::string, std::string> mimetypes, std::string content_type);

/* utils */
bool checkIndexInsidDerctory(std::string *path);
const std::string generatBody(std::string _requestedSource);
int calculeBodySize(std::string _requestedSource); // make this return long
std::string generateAutoindexFile(std::string requestedSource); /* in GET method */
std::string getCurrentDate();
std::string trim(const std::string& str);
std::pair<std::string, std::string> parseHeader(const std::string& line);
std::string searchInRequestedHeader(const std::map<std::string, std::string>& Map, const std::string& key);

template <typename T>
T toNumber(const std::string& str) {
    T result;
    std::istringstream iss(str);
    iss >> result;
    return result;
}

// error page 
const std::string GenerateErrorPage(int statuscode, std::string statusmessage);
std::string SearchAboutErrorPageFormTowPlaces(int statuscode, std::map<int, std::string> g_err_page, std::map<int, std::string> l_err_page);
std::string SearchAboutErrorPage(int statuscode, std::map<int, std::string> g_err_page);
std::string ReadErrorPage(std::string errpage);











// -----------------------------------------------------------------------------------------------------------------------------------------------
							/*   data     */
//  the Request class
// class Request
// {
//     private:
// 		std::string _method;
// 		std::string _uri;
// 		std::string _version;
//         std::string _conection;
// 		std::map<std::string, std::string> _headers;
// 		std::string _body;
//     public:
// 		Request(){
//             _method = "GET";
//             _uri = "/app/html/index.html";
//             _version = "HTTP/1.1";
//             _conection = "close";
// 			_headers[""] = "";
//         }
//         std::string getMethod() const {
//             return _method;
//         }
//         std::string getUri() const {
//             return _uri;
//         }
//         std::string getVersion() const
//         {
//             return _version;
//         }
//         std::string getConection() const {
//             return _conection;
//         }
// 		~Request(){}
// };

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
