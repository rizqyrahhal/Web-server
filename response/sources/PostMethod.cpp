/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 16:45:33 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/17 04:30:45 by rarahhal         ###   ########.fr       */
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
        // FILL all this on toow function upload_data_managememnt() and creatin_file()
        
        // getFileName(); use searchInRequestedHeader() if set in header or take it from requset ase _filename
        // gitextantion(); make this function that tak map of _mimetype and searech by Conetent-Type about extantion
        // int file = open((getFileName() + gitextantion()), WITH flag to creat); // me be use ifstream to be sample to appand in file

        std::ifstream infile("videoplayback.mp4",std::ifstream::binary);
        std::ofstream outfile ("filename.mp4",std::ofstream::binary);

        int size = calculeBodySize("videoplayback.mp4");
        /* using vector becouse when use the char* the binry have an \0 in your content */
        std::vector<char> buffer(size + 1);
        infile.read(&buffer[0],size);
        buffer[size] = '\0';
        outfile.write(&buffer[0],size);
        outfile.close();
        infile.close();

        // // open not allowed
        // int fd = open("filename.mp4", O_TRUNC | O_CREAT | O_EXCL | O_RDWR, 0777); // O_EXCL -->(error if O_CREAT and the file exists)
        // if (fd == -1)
        //     throw(500);
        // char buffer[1025];
        // int size_read;
        // while((size_read = read(request.bodyFile, buffer, 1024)) > 0) {
        //     buffer[size_read] = '\0';
        //     int size_write = write(fd, buffer, size_read);
        //     std::cout << "I write this size to filename.png :->> " << size_write << std::endl;
        // }
        // close(fd);

        // throw(500); // becous when return 201 it is a probleme where i dont new but need to fixe   (check in the both floow of 500 and 201 response creation )
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
