/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetMethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:24:42 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/23 22:34:01 by rarahhal         ###   ########.fr       */
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
    _requestedSource = Response::GetRequestedSource(server.locations[Response::_matchedLocationPosition], request.url);
    if (DEBUG) {
        std::cout << "_requestedSource: " << _requestedSource << std::endl;
    }
    _resourceType = Response::GetResourceType(_requestedSource);
    if (DEBUG) {
        std::cout << "_resourceType: " << _resourceType << std::endl;
    }

    return ("");
}