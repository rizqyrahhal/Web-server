/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 04:03:03 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/13 04:10:23 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

ResponseReturned::ResponseReturned() {
    /* open and set up the file */
    // set up the needed data
    // set up the static reding variabels
}
std::string ResponseReturned::GetChanckFromResponse(size_t size_to_read) {
	(void)size_to_read;
//     if (!_headers.empty())
//     {
//         std::string headToSend(_headers);
//         _headers.clear();
//         return(headtosend); // in the first requsted chunck
//     }
//      /* then send headers */
//     if (!_body.empty())
//     {
//         if(isFile) {
//             /* open and set up the file */ //my be in the constructer
//             readfromfile();
//         }
//         else
//             readfromstring(size_to_read);
//     }
    return (""); // when return the Empty string the response is finish sending
}

/* Seters */

void ResponseReturned::setHeaders(std::string headers) {
    _headers = headers;
}

void ResponseReturned::setBody(std::string &body) {
    _body = body;
}

void ResponseReturned::setIsFile(bool &isfile) {
    _isFile = isfile;
}

/* Geters */

std::string ResponseReturned::getHeaders() const {
    return _headers;
}

std::string ResponseReturned::getBody() const {
    return (_body);
}

bool ResponseReturned::getIsFile() const {
    return (_isFile);
}

ResponseReturned::~ResponseReturned() {
    // close the file and desrtoy any data
}