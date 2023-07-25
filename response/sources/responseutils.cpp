/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseutils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araysse <araysse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:28:22 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/25 15:21:38 by araysse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

void Response::checkForIndexFile(Response *response, server server, std::string &bodyfile, bool &isfile) {
    if (server.locations[response->_matchedLocationPosition].index.empty())
    {
        if (checkIndexInsidDerctory(&response->_requestedSource)) /* return bool and append index.html to requstedSource if existe */
            return;
        if (server.locations[response->_matchedLocationPosition].autoindex == "on")
        {
            /* generate outoindex */
            bodyfile = generateAutoindexFile(response->_requestedSource);
            isfile = false;
            throw(200);
        }
        else
            throw(403);
    }
    if (!server.locations[response->_matchedLocationPosition].index.empty())
    {
        response->_requestedSource = server.locations[_matchedLocationPosition].index;
        return ;
    }
}

bool Response::isCgi() {
    for (std::map<std::string, std::string>::iterator it = _cgi_mimeTypes.begin(); it != _cgi_mimeTypes.end(); it++) {
        if (_contentType == it->second)
            return true;
    }
    return false;
}

void Response::parseCgiOutput( const std::string& cgioutput) {
	std::string _line;
	size_t _seek = 0;

    if (cgioutput.empty())
        throw(502);

    std::istringstream iss(cgioutput);

	while (std::getline(iss, _line)) {
		_seek += _line.length() + 1;
		_line = trim(_line);
		if (_line.size() <= 1)
            break;
		std::pair<std::string, std::string> _header = parseHeader(_line);
	    if (_header.first == "X-Powered-By" || _header.first == "Status") {
		    if (_header.first == "Status")
			    setStatusCode(toNumber(_header.second));
	    }
        else if (_header.first == "Content-type" || _header.first == "Content-Type")
			continue;
	    else
			setHeader(_header.first, _header.second);
	}
    setHeader("Content-Type", "text/html");

    if (!_status_code || _status_code == 200)
        _status_code = 200;
    else {
        throw(502);
    }

	if (cgioutput.size() <= _seek) {
		setHeader("Content-Length", "0");
		throw(_status_code);
	}

    setBody(std::string(cgioutput.begin() + _seek, cgioutput.end()) + "\0");
    setHeader("Content-Length", std::to_string(std::string(cgioutput.begin() + _seek, cgioutput.end()).size()));
    isfile = false;
}

void Response::GetContentType(std::string requestedSource, std::map<std::string, std::string> mimetypes, std::string &contenttype) {
    contenttype = getMimeType(mimetypes, getFileExtantion(requestedSource));
    if (contenttype.empty())
        contenttype = "text/plain";
}

std::string Response::searchInRoot(locations matchedlocation, std::string requesturi, bool &resourcetype, Response *response, std::string method) {
    DIR *dir;
	std::string root = matchedlocation.root;
	size_t position;
	std::string requestedSource;

    std::string uri = requesturi;

    uri.erase(1, matchedlocation.name.size()-1);

    while (uri.size())
    {
        if (uri == root || uri == "/")
        {
            requestedSource = root;
            /* check if exist */
            dir = opendir(requestedSource.c_str());
            if (dir != NULL) {
                resourcetype = DRCT;
                closedir(dir);
        	    return (requestedSource);
            }
            else if (access(requestedSource.c_str(), 0) == 0) {
                resourcetype = FILE;
        	    return (requestedSource);
            }
        }
		requestedSource = root + uri;

        /* check if exist */
        dir = opendir(requestedSource.c_str());
        if (dir != NULL) {
            resourcetype = DRCT;
            if (requestedSource[requestedSource.size() - 1] != '/') {
                if (method == "DELETE") {
                    closedir(dir);
                    throw (409);
                }
                response->setHeader("Location", requesturi+"/");
                closedir(dir);
                throw(301);
            }
            closedir(dir);
        	return (requestedSource);
        }
        else if (access(requestedSource.c_str(), 0) == 0) {
            resourcetype = FILE;
        	return (requestedSource);
        }

		position = uri.find_first_of("/");
		if (position == std::string::npos)
   			throw(404);
		uri.erase(0, position+1);
    }

    throw(404);
}

std::string Response::GetRequestedSource(locations matchedlocation, locations rootlocation, std::string requesturi, bool &resourcetype, Response *response, std::string method) {
    DIR *dir;
	std::string root = matchedlocation.root;
	size_t position;
	std::string requestedSource;

    requesturi = removeReturnBack(requesturi);

    std::string uri = requesturi;

    uri.erase(1, matchedlocation.name.size()-1);

    while (uri.size())
    {
        if (uri == root || uri == "/")
        {
            requestedSource = root;
            /* check if exist */
            dir = opendir(requestedSource.c_str());
            if (dir != NULL) {
                resourcetype = DRCT;
                closedir(dir);
        	    return (requestedSource);
            }
            else if (access(requestedSource.c_str(), 0) == 0) {
                resourcetype = FILE;
        	    return (requestedSource);
            }
        }
		requestedSource = root + uri;

        /* check if exist */
        dir = opendir(requestedSource.c_str());
        if (dir != NULL) {
            resourcetype = DRCT;
            if (requestedSource[requestedSource.size() -1] != '/') {
                if (method == "DELETE") {
                    closedir(dir);
                    throw (409);
                }
                response->setHeader("Location", requesturi+"/");
                closedir(dir);
                throw(301);
            }
            closedir(dir);
        	return (requestedSource);
        }
        else if (access(requestedSource.c_str(), 0) == 0) {
            resourcetype = FILE;
        	return (requestedSource);
        }

		position = uri.find_first_of("/");
		if (position == std::string::npos) {
   			break;
        }
		uri.erase(0, position + 1);
    }
    std::string requestedSourceFromRoot = searchInRoot(rootlocation, requesturi, resourcetype, response, method);

    throw(404);
}