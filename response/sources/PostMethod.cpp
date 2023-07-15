/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 16:45:33 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/15 00:40:09 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"
bool supportUpload(locations matchedlocation) {
    if (!matchedlocation.upload_pass.empty())
        return true;
    return false;
}

void Response::PostMethod(server server, request request) {
    #ifdef DEBUG	
    	std::cout << "Upload_pass: " << server.locations[_matchedLocationPosition].upload_pass\
            << std::endl << "Requested Method: " << request.method << std::endl;
    #endif
    /* in this condition setup the position to upload the body of request */
    if(supportUpload(server.locations[_matchedLocationPosition])) {
        // getFileName(); genarate or detect the filename

		//  tafrigh mo7tawa uploaded file to the filename in the needed place
        throw(201);
    }
    /* here the other way, working on location dosn't support upload */
    else {
        _requestedSource = Response::GetRequestedSource(server.locations[Response::_matchedLocationPosition], request.url, _resourceType, &(*this), request.method);

		#ifdef DEBUG
        std::cout << "_requestedSource: " << _requestedSource << std::endl;
        std::cout << "_resourceType: ";
        if(_resourceType)
            std::cout << "FILE\n";
        else
            std::cout << "DERCTORY\n";
        #endif
        Response::GetContentType(_requestedSource, _mimeTypes, _contentType);
        #ifdef DEBUG
            std::cout << "contentType : " << _contentType << std::endl;
        #endif
        if (_resourceType == DRCT && !checkIndexInsidDerctory(&_requestedSource)) {
			throw(403);
        }
		else if (server.locations[_matchedLocationPosition].cgi.empty())
			throw(403);
		else {
            std::cout << "--------- Me by here ----------\n";
			/* run cgi on requested file with POST request_method */
			cgi(server, request);
        }
    }
}
