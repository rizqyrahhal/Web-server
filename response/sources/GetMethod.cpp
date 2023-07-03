/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetMethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:24:42 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/03 15:41:44 by rarahhal         ###   ########.fr       */
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

/* this function for new return a response inside string,
    but for advance return object that contian header indide string,
    and file of body and member function to red from this file by spesific size */
void Response::GetMethod(server server, request request) {
    _requestedSource = Response::GetRequestedSource(server.locations[Response::_matchedLocationPosition], request.url, _resourceType, &(*this), request.method);
    #ifdef DEBUG
        std::cout << "_requestedSource: " << _requestedSource << std::endl;
        std::cout << "_resourceType: ";
        if(_resourceType)
            std::cout << "FILE\n";
        else
            std::cout << "DERCTORY\n";
    #endif
    if (_resourceType == DRCT || _resourceType == FILE)
    {
        if (_resourceType == DRCT){
            checkForIndexFile(&(*this), server);
        }
        Response::GetContentType(_requestedSource, _mimeTypes, _contentType);
        setHeader("Content-Type", _contentType);
        #ifdef DEBUG
            std::cout << "contentType : " << _contentType << std::endl;
        #endif
        setBody(generatBody(_requestedSource));
        throw(200);
    }
    else if (_resourceType != DRCT && _resourceType != FILE) {
                    //cgi
        /* my be function to excute cgi if file or directory have cgi */
    }
}

// <!-- DOCTYPE html -->
// <html>
// <head><title>Directory Listing</title></head>
// <body>
// <h1>Directory Listing</h1>
// <ul>
// <li><a href="./www/app/html/.">.</a></li>
// <li><a href="./www/app/html/..">..</a></li>
// <li><a href="./www/app/html/images">images</a></li>
// <li><a href="./www/app/html/logo.png">logo.png</a></li>
// <li><a href="./www/app/html/Music.mp4">Music.mp4</a></li>
// <li><a href="./www/app/html/scripts">scripts</a></li>
// <li><a href="./www/app/html/styles">styles</a></li>
// <li><a href="./www/app/html/videoplayback.mp4">videoplayback.mp4</a></li>
// </ul>
// </body>
// </html>