/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 11:09:55 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/28 01:10:17 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

void Response::DeleteMethod(server server, request request) {
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
    if (_resourceType == DRCT || _resourceType == FILE) {  // change this condition in futere
        if (_resourceType == DRCT){ // directory handling
            checkForIndexFile(&(*this), server);
        }
        /* file handling */
        #ifdef CURENT_DEBUG
            std::cout << "_requestedSource: " << _requestedSource << std::endl;
        #endif
		if (std::remove(_requestedSource.c_str()) == 0)
			throw (204);
    }
    else if (_resourceType != DRCT && _resourceType != FILE) {
                    //cgi
        /* my be function to excute cgi if file or directory have cgi */
    }
}
