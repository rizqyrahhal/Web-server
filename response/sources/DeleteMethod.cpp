/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 11:09:55 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/27 13:21:06 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

std::string Response::DeleteMethod(server server, request request) {
    _requestedSource = Response::GetRequestedSource(server.locations[Response::_matchedLocationPosition], request.url, _resourceType, &(*this), request.method);
    
    #ifdef DEBUG
        std::cout << "_requestedSource: " << _requestedSource << std::endl;
        std::cout << "_resourceType: ";
        if(_resourceType)
            std::cout << "FILE\n";
        else
            std::cout << "DERCTORY\n";
    #endif
    /* affter finish dyrectory error if no error and index take the same floow like file just set the neded information */
    if (_resourceType == DRCT || _resourceType == FILE)
    {
        if (_resourceType == DRCT){
            checkForIndexFile(&(*this), server);
        }
        Response::GetContentType(_requestedSource, _mimeTypes, _contentType);
        setHeader("Content-Type", _contentType);
        setBody(generatBody(_requestedSource));
        #ifdef DEBUG
            std::cout << "contentType : " << _contentType << std::endl;
        #endif
        throw(200);
    }
    else if (_resourceType != DRCT && _resourceType != FILE) {
                    //cgi
        /* my be function to excute cgi if file or directory have cgi */
    }


    return ("");
    return ("");
}
