/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 16:45:33 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/20 19:43:25 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"
bool supportUpload(locations matchedlocation) {
    if (!matchedlocation.upload_pass.empty())
        return true;
    return false;
}

std::string creatNewName(std::string upload_pass, std::string file_name) {
    if (upload_pass.empty()) { // this condition Just if switch upload_pass to on for new non working
        return("www/upload/" + file_name);
    }
    if (!upload_pass.find("/", upload_pass.size())) {
        return (upload_pass + file_name);
    }
    else
        return (upload_pass + "/" + file_name);
}

void Response::PostMethod(server server, request request) {
    #ifdef POST_DEBUG
    	std::cout << "Upload_pass: " << server.locations[_matchedLocationPosition].upload_pass\
            << std::endl << "Requested Method: " << request.method << std::endl;
    #endif

    /* in this condition setup the position to upload the body of request */
    if(supportUpload(server.locations[_matchedLocationPosition])) {

        //         data hardcoded  to upload            //
        // std::string content_type = "video/mp4";
        // std::string file_name = "upload_name";        // when test upload take in your maind that is hard code and the 201 have probleme
        // std::string tompprare_name = "www/tmp1";
        ////////////////////////////////////////////////// 
        std::string file_name = searchInRequestedHeader(request.map_request, "File-Name");   /// need to make this 

        


        #ifdef UPLOAD_DEBUG
            // std::cout << "Extantio: " << getExtantion(_mimeTypes, content_type) << std::endl;
            std::cout << "Extantio: " << getExtantion(_mimeTypes, searchInRequestedHeader(request.map_request, "Content-Type")) << std::endl;
            std::cout << "New Name: " << creatNewName(server.locations[_matchedLocationPosition].upload_pass, (file_name + getExtantion(_mimeTypes, searchInRequestedHeader(request.map_request, "Content-Type")))) << std::endl;
            // std::cout << "New Name: " << creatNewName(server.locations[_matchedLocationPosition].upload_pass, (file_name + getExtantion(_mimeTypes, content_type))) << std::endl;
        #endif

        // if (!rename(tompprare_name.c_str() , creatNewName(server.locations[_matchedLocationPosition].upload_pass, (file_name + getExtantion(_mimeTypes, content_type))).c_str()))
        if (!rename(request.bodyfile_name.c_str() , creatNewName(server.locations[_matchedLocationPosition].upload_pass, (file_name + getExtantion(_mimeTypes, searchInRequestedHeader(request.map_request, "Content-Type")))).c_str()))
            throw(201);
        else
            throw(500);


        // std::ifstream infile("bodyFile",std::ifstream::binary);
        // std::ofstream outfile ("filename.mp4",std::ofstream::binary);

        // int size = calculeBodySize("bodyFile");

        /* using vector becouse when use the char* the binry have an \0 in your content */
        // std::vector<char> buffer(size + 1);
        // infile.read(&buffer[0],size);
        // buffer[size] = '\0';
        // outfile.write(&buffer[0],size);
        // outfile.close();
        // infile.close();

        // throw(500); // becous when return 201 it is a probleme where i dont new but need to fixe   (check in the both floow of 500 and 201 response creation )
        // throw(201);
    }
    /* here the other way, working on location dosn't support upload */
    else {
        _requestedSource = Response::GetRequestedSource(server.locations[Response::_matchedLocationPosition], request.url, _resourceType, &(*this), request.method);
        Response::GetContentType(_requestedSource, _mimeTypes, _contentType); // this is is change from after directory to beffor

		#ifdef POST_DEBUG
        std::cout << "_requestedSource: " << _requestedSource << std::endl;
        std::cout << "_resourceType: ";
        if(_resourceType)
            std::cout << "FILE\n";
        else
            std::cout << "DERCTORY\n";
        std::cout << "contentType : " << _contentType << std::endl;
        #endif

        if (_resourceType == DRCT && !checkIndexInsidDerctory(&_requestedSource)) {
			throw(403);
        }
		else if (server.locations[_matchedLocationPosition].cgi.empty())
			throw(403);
		else {
			/* run cgi on requested file with POST request_method */
			cgi(server, request, *this);
            // throw(1);
        }
    }
}