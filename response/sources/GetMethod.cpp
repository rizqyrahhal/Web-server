/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetMethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:24:42 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/26 04:43:43 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"


/* this function for new return a response inside string,
    but for advance return object that contian header indide string,
    and file of body and member function to red from this file by spesific size */
std::string Response::GetMethod(server server, request request) {
    // (void)server;
    // (void)request;

    // !!!!!!!!!!!!!! STOP HERE but path need more and more handling !!!!!!!!!!!!!!!
    _requestedSource = Response::GetRequestedSource(server.locations[Response::_matchedLocationPosition], request.url, _resourceType, *this);

    #ifdef DEBUG
        std::cout << "_requestedSource: " << _requestedSource << std::endl;
        std::cout << "_resourceType: ";
        if(_resourceType)
            std::cout << "FILE\n";
        else
            std::cout << "DERCTORY\n";
    #endif
    if (_resourceType == DRCT)
    {
        //dyrctoryhandler//
    }
    else if (_resourceType == FILE)
    {
        //fileshandler//
        Response::GetContentType(_requestedSource, _mimeTypes, _contentType);
        setHeader("Content-Type", _contentType);
        setBody(generatBody(_requestedSource));
        throw(200);
    }

    #ifdef DEBUG
    std::cout << "contentType : " << _contentType << std::endl;
    #endif

    return ("");
}