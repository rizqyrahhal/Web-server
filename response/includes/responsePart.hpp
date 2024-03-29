/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePart.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 15:35:12 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/25 01:18:36 by rarahhal         ###   ########.fr       */
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
// #define DEBUG
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
// #define UPLOAD_DEBUG
#endif

#ifndef CGI_DEBUG
// #define CGI_DEBUG
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
	    static size_t _matchedLocationPosition;
		std::string _matchedLocation;
		std::string _requestedSource;
		bool _resourceType; /* dyrictore Vs file */
		std::string _contentType;
		std::string _method;

		/* Cgi*/
		std::string _cgiBinPath;
		char **_envp;
		char **_argv;

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
		static std::string GetRequestedSource(locations matchedlocation, locations rootlocation, std::string requesturi, bool &resourcetype, Response *response, std::string method);
		static std::string searchInRoot(locations matchedlocation, std::string requesturi, bool &resourcetype, Response *response, std::string method);
		static void checkForIndexFile(Response *response, server server, std::string &bodyfile, bool &isfile);
	
	public:
		Response();
		ResponseReturned CreatResponse(server server, request request);
		~Response();
	private: /* switched data */
		std::string bodyfile;
		bool isfile;
};

std::string GenerateResponseFromStatusCode(int statuscode, server server);

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
std::string getRandomString(int length);
int toNumber(const std::string& str);
std::string removeReturnBack(std::string url);



/* Error Page */
const std::string GenerateErrorPage(int statuscode, std::string statusmessage);
std::string SearchAboutErrorPageFormTowPlaces(int statuscode, std::map<int, std::string> g_err_page, std::map<int, std::string> l_err_page);
std::string SearchAboutErrorPage(int statuscode, std::map<int, std::string> g_err_page);
std::string ReadErrorPage(std::string errpage);