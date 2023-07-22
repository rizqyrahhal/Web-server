/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseutils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:28:22 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/22 22:36:11 by rarahhal         ###   ########.fr       */
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
			    setStatusCode(toNumber<int>(_header.second));
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

    setBody(std::string(cgioutput.begin() + _seek, cgioutput.end()));
    setHeader("Content-Length", std::to_string(std::string(cgioutput.begin() + _seek, cgioutput.end()).size()));
    isfile = false;
}

void Response::GetContentType(std::string requestedSource, std::map<std::string, std::string> mimetypes, std::string &contenttype) {
    contenttype = getMimeType(mimetypes, getFileExtantion(requestedSource));
    if (contenttype.empty())
        contenttype = "text/plain";
}

std::string Response::GetRequestedSource(locations matchedlocation, std::string requesturi, bool &resourcetype, Response *response, std::string method) {
    // std::cout << "REQUESTED uri: " << requesturi << std::endl << std::endl;

    DIR *dir;
	std::string root = matchedlocation.root;
    std::string uriplusslash = requesturi + "/";
	size_t position;
	std::string requestedSource;
	


    // std::cout << "########## ENTREE INSIDE THE LOOP\n";
    while (root.size())
    {
		requestedSource = root + requesturi;
		// std::cout << "ROOT + URI : " << requestedSource << std::endl;
        if (requesturi == root || requesturi == "/")
        {
            /* check if exist */
            dir = opendir(requestedSource.c_str());
            if (dir != NULL) {
                resourcetype = DRCT;
                // if (requestedSource[requestedSource.size() - 1] != '/') {
                //     if (method == "DELETE")
                //         throw (409);
                // response->setHeader("Location", uriplusslash);
                // throw(301);
                // }
                closedir(dir);
        	    return (requestedSource);
            }
            else if (access(requestedSource.c_str(), 0) == 0) {
                resourcetype = FILE;
        	    return (requestedSource);
            }
        }
		
        /* check if exist */
        dir = opendir(requestedSource.c_str());
        if (dir != NULL) {
            resourcetype = DRCT;
            if (requestedSource[requestedSource.size() - 1] != '/') {
                if (method == "DELETE") {
                    closedir(dir);
                    throw (409);
                }
                response->setHeader("Location", uriplusslash);
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

		position = root.find_last_of("/");
        // std::cout << "positionINloop: " << position << std::endl << std::endl;
		if (position == std::string::npos)
   			throw(404);	
		root.erase(position, root.size());
		// std::cout << "ROOT NEW: " << root << std::endl;
    }
    // std::cout << "########## GO OUT THE LOOP\n";
    // std::cout << "requestedSource: " << requestedSource << "\n";
    throw(404);
}