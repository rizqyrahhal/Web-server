/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 11:09:55 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/05 23:25:29 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

bool writeAccess(std::string requestedSource) {
	if (access(requestedSource.c_str(), W_OK) == 0)
	    return (true);
    else
        return (false);
}

bool deleteDirectory(std::string requestedSource) {
    DIR* dr = opendir(requestedSource.c_str());
    if (dr != NULL)
    {
        struct dirent *dir = NULL;
        while ((dir = readdir(dr))) {
			std::string name(dir->d_name);
			if (name == "." || name == "..")
				continue;
			if (opendir((requestedSource + name + "/").c_str()) != NULL)
				if (!deleteDirectory(std::string(requestedSource + name + "/")))
					return (false);
			if (std::remove((requestedSource + name).c_str()) == 0)
				continue;
			else
			{
        		closedir(dr);
                if (!writeAccess(requestedSource))
					throw(304);
				else
					throw(500);
			}
		}
        closedir(dr);
    }
    else {
        if (!writeAccess(requestedSource))
			throw(304);
		else
			throw(500);
    }
    return (true);
}

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
        
		/* directory handling */
		if (_resourceType == DRCT) {
            // if (location has cgi ) {
            //     checkForIndexFile(&(*this), server);
            // }
			if (deleteDirectory(_requestedSource))
				throw(204);
        }

        /* file handling */
        // if (location has cgi ) {
        //     cgi code
        // }
		if (std::remove(_requestedSource.c_str()) == 0)
			throw (204);
    }
    else if (_resourceType != DRCT && _resourceType != FILE) {
                    //cgi
        /* my be function to excute cgi if file or directory have cgi */
    }
}










/* the First implementation of this algo */
// bool deleteDirectory(std::string requestedSource) {
//     std::cout << "-------- ------------- :" << requestedSource << std::endl;
//     bool why = true;
//     DIR* dr = opendir(requestedSource.c_str());
//     if (dr != NULL)
//     {
//         struct dirent *dir = NULL;
//         while ((dir = readdir(dr))) {
// 			std::string name(dir->d_name);
// 			if (name == "." || name == "..")
// 				continue;
// 			if (opendir((requestedSource + name + "/").c_str()) != NULL)
// 			{
// 				if (!deleteDirectory(std::string(requestedSource + name + "/")))
// 					why = false;
// 			}
// 			if (std::remove((requestedSource + name + "/").c_str()) == 0)
// 				continue;
// 			else
// 			{
//         		// closedir(dr);
// 				why = false;
// 			}
// 		}
//         // closedir(dr);
//     }
//     else
//     {
//         std::cout << "ERROR IN OPENING DIRECTORY (this error be to check about it in parsing!!!)";
//         return false;
//     }
//     return (true);
// }