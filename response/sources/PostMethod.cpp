/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 16:45:33 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/16 02:38:58 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"
bool supportUpload(locations matchedlocation) {
    if (!matchedlocation.upload_pass.empty())
        return true;
    return false;
}


    // struct stat fileInfo;
    // if (fstat(fileDescriptor, &fileInfo) == -1) {
    //     // Error handling for fstat error
    //     std::cout << "Failed to get file information." << std::endl;
    //     close(fileDescriptor); // Close the file descriptor
    //     return 1;
    // }

    // off_t fileSize = fileInfo.st_size;

void Response::PostMethod(server server, request request) {
    #ifdef DEBUG
    	std::cout << "Upload_pass: " << server.locations[_matchedLocationPosition].upload_pass\
            << std::endl << "Requested Method: " << request.method << std::endl;
    #endif
    /* in this condition setup the position to upload the body of request */
    if(supportUpload(server.locations[_matchedLocationPosition])) {
        // getFileName(); use searchInRequestedHeader() if set in header or take it from requset ase _filename
        // gitextantion(); make this function that tak map of _mimetype and searech by Conetent-Type about extantion
        // int file = open((getFileName() + gitextantion()), WITH flag to creat); // me be use ifstream to be sample to appand in file

        int fd = open("filename.png", 777);
        std::vector<char> buffer(2597);
        int size_read = read(request.bodyFile, &buffer[0], 2597);
        std::cout << "I read this size to bodysfile" << size_read << std::endl;

        std::cout << "I take thiis content from file descriptor:\n" << std::string(buffer.begin(), buffer.end()) << std::endl;
        int size_write = write(fd, std::string(buffer.begin(), buffer.end()).c_str(), 2597);
        // int size_write = write(fd, &buffer[0], 2597);
        std::cout << "I write this size to filename.png" << size_write << std::endl;
        (void)fd;
		//  tafrigh mo7tawa uploaded file to the filename in the needed place
        throw(500); // becous when return 201 it is a probleme where i dont new but need to fixe   (check in the both floow of 500 and 201 response creation )
        // throw(201);
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
