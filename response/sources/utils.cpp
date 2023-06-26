/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:28:22 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/26 04:51:02 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"


const std::string generatBody(std::string _requestedSource) {
    std::ifstream file(_requestedSource.c_str(), std::ifstream::binary);
    if (!file.is_open())
    {
    	std::cout << "Error in opening file\n";
    	std::exit(-404);
    }
    file.seekg(0, std::ios::end);
    int length = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(length);

    // char *buffer = new char[length + 1];
    file.read(&buffer[0], length);
	// buffer[length] = '\0';

    file.close();

    // std::cout << "-----------: 1\n" << buffer << "\n";
    // std::cout << "length: " << length << std::endl;
    
    return (std::string(buffer.begin(), buffer.end()));
    // return (buffer);
}

void Response::GetContentType(std::string requestedSource, std::unordered_map<std::string, std::string> mimetypes, std::string &contenttype) {
    contenttype = getMimeType(mimetypes, getFileExtantion(requestedSource));
}

// std::string Response::GetResourceType(std::string requestedSource) {
//     (void)requestedSource;
//         if (access(requestedSource.c_str(), 0) == 0)
//         {
//             // checkextention
//         }
//         if (opendir(requestedSource.c_str()) != NULL)
//         {
//             std::cout << "----------------- " << requestedSource << std::endl;
//         }

//     // if ((requestedSource.size()-1) == )
//     return ("");
// }


/* this function check the resource existent and its type whit rediriction if the type directory and hav'not / in the end
 {!!!}new it is work but in simple casae  */
std::string Response::GetRequestedSource(locations matchedlocation, std::string requesturi, bool &resourcetype, Response response) {
    size_t position = requesturi.find_last_of("/");
    std::string checked, requestedSource, uriplusslash;

    // if (DEBUG) {
    //     std::cout << "matchedlocation.name: " << matchedlocation.name << std::endl;
    //     std::cout << "matchedlocation.root: " << matchedlocation.root << std::endl;
    //     std::cout << "requesturi: " << requesturi << std::endl;
    //     std::cout << "positionAFfterloop: " << position << std::endl << std::endl;
    // }

    while (position != 0)
    {
        checked = requesturi.substr(position, requesturi.size());
        requestedSource = "." + matchedlocation.root + checked;
        uriplusslash = requesturi + "/";

        // if (DEBUG) {
        //     std::cout << "checked: " << checked << "\n";
        //     std::cout << "requestedSource: " << requestedSource << "\n";
        //     std::cout << "uriplusslash: " << uriplusslash << "\n";
        //     std::cout << "positionINloop: " << position << std::endl << std::endl;
        // }
    
        /* check if exist */
        if (opendir(requestedSource.c_str()) != NULL) {
            resourcetype = DRCT;
            if (requestedSource[requestedSource.size() - 1] != '/') {
                // HttpResponse response;
                response.setHeader("Location", uriplusslash);
                // throw(GenerateResponseFromStatusCode(301, response));
                throw(301);
            }
        	return (requestedSource);
        }
        else if (access(requestedSource.c_str(), 0) == 0) {
            resourcetype = FILE;
        	return (requestedSource);
        }
		position = requesturi.find_last_of("/", position - 1);
    }

    if (matchedlocation.name == requesturi)
    { 
        /* check if exist */
        if (opendir(requestedSource.c_str()) != NULL) {
            resourcetype = DRCT;
            if (requestedSource[requestedSource.size() - 1] != '/') {
                // HttpResponse response;
                response.setHeader("Location", uriplusslash);
                // throw(GenerateResponseFromStatusCode(301, response));
                throw(301);
            }
        	return (requestedSource);
        }
        else if (access(requestedSource.c_str(), 0) == 0) {
            resourcetype = FILE;
        	return (requestedSource);
        }
    }
    // throw(GenerateResponseFromStatusCode(404));
    throw(404);
}






















// std::string Response::GetRequestedSource(locations matchedlocation, std::string requesturi) {
//     size_t position = requesturi.find_last_of("/");
//     std::string checked, requestedSource;
//     while (position != 0)
//     {
//         checked = requesturi.substr(position, requesturi.size());
//         requestedSource = "." + matchedlocation.root + checked;
 
//         /* check if exist */
//         if (access(requestedSource.c_str(), 0) == 0)
//         {
//         	return (requestedSource);
//         }
//         else if (opendir(requestedSource.c_str()) != NULL)
//         {
//         	return (requestedSource);
//         }
// 		position = requesturi.find_last_of("/", position - 1);
//     }
//     if (matchedlocation.name == requesturi)
//     { 
//         if (access(requestedSource.c_str(), 0) == 0)
//         {
//         	return (requestedSource);
//         }
//         else if (opendir(requestedSource.c_str()) != NULL)
//         {
//         	return (requestedSource);
//         }
//     }
//     throw(GenerateResponseFromStatusCode(404));
// }