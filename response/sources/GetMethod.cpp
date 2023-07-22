/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetMethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:24:42 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/22 03:22:03 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

std::string generateAutoindexFile(std::string requestedSource) {
    
    std::string indexhtml = "<!-- DOCTYPE html --><html><head><title>Directory Listing</title></head><body><h1>Directory Listing</h1><ul>";
    DIR* dr = opendir(requestedSource.c_str());
    
    if (dr != NULL)
    {
        struct dirent *dir = NULL;
        while ((dir = readdir(dr)))
            indexhtml += "<li><a href=\"" + std::string(dir->d_name) + "\">" + dir->d_name + "</a></li>";
        indexhtml += "</ul></body></html>";
        closedir(dr);
    }
    else
        std::cout << "ERROR IN OPENING DIRECTORY (this error be to check about it in parsing!!!)";
    return (indexhtml);
}

void Response::GetMethod(server server, request request, std::string &bodyfile, bool &isfile) {
    _requestedSource = Response::GetRequestedSource(server.locations[Response::_matchedLocationPosition], request.url, _resourceType, &(*this), request.method);

    #ifdef GET_DEBUG
        std::cout << "_requestedSource: " << _requestedSource << std::endl;
        std::cout << "_resourceType: ";
        if(_resourceType)
            std::cout << "FILE\n";
        else
            std::cout << "DERCTORY\n";
    #endif

	if (_resourceType == DRCT) {
        checkForIndexFile(&(*this), server, bodyfile, isfile);
    
        #ifdef GET_DEBUG
            std::cout << "_requestedSource: " << _requestedSource << std::endl;
        #endif
    }

    Response::GetContentType(_requestedSource, _mimeTypes, _contentType);

    #ifdef GET_DEBUG
        std::cout << "contentType : " << _contentType << std::endl;
    #endif

    if (isCgi()) {
        if (!server.locations[_matchedLocationPosition].cgi.empty()) {
            std::cout << "WACH HNA :::::::::::::::::::::::::::::::::::::::::::::::\n";
            cgi(server, request, *this);
        }
        _contentType = "text/plain";
    }
	// if (_contentType == "application/x-httpd-php" || _contentType == "application/x-python-code")
	// 	cgi(server, request);
	// if (_contentType.empty())
	// 	cgi(server, request);

    setHeader("Content-Type", _contentType);
	setHeader("Content-Length", std::to_string(calculeBodySize(_requestedSource)));
	bodyfile = _requestedSource;
	isfile = true;
    throw(200);
}
