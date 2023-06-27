/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseutils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:28:22 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/27 00:07:23 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

void Response::checkForIndexFile(Response *response, server server) {
    std::cout << "requestedSource: --=++: " << response->_requestedSource << std::endl;
    std::cout << "serverName: --=++: " << server.server_name << std::endl;
    std::cout << "autoindex: --+++: " << server.locations[response->_matchedLocationPosition].autoindex << std::endl;
    if (server.locations[response->_matchedLocationPosition].index.empty())
    {
        if (checkIndexInsidDerctory(&response->_requestedSource)) // return bool
            return;
        if (server.locations[response->_matchedLocationPosition].autoindex == "on")
        {
            here /// //  // / / / / / / / / / // / / / / // / / / / / / //////////////////////////////////////// // / / / /
            // generate outoindex
        }
        else
            throw(403);
    }
    if (!server.locations[response->_matchedLocationPosition].index.empty())
    {
        response->_requestedSource = server.locations[_matchedLocationPosition].index;
        return ;
    }
}

void Response::GetContentType(std::string requestedSource, std::unordered_map<std::string, std::string> mimetypes, std::string &contenttype) {
    contenttype = getMimeType(mimetypes, getFileExtantion(requestedSource));
}


/* this function check the resource existent and its type whit rediriction if the type directory and hav'not / in the end
 {!!!}new it is work but in simple casae  
 {!!!} case if have root /www/app and the url /www/app/html/ the source generate is /www/app/ ERROR the correct is /www/app/html/
 */
std::string Response::GetRequestedSource(locations matchedlocation, std::string requesturi, bool &resourcetype, Response *response) {
    size_t position = requesturi.find_last_of("/");
    std::string checked, requestedSource, uriplusslash;
    uriplusslash = requesturi + "/";

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
                response->setHeader("Location", uriplusslash);
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

    if (matchedlocation.root == requesturi || matchedlocation.name == requesturi)
    {
        requestedSource = "." + requesturi;
        /* check if exist */
        if (opendir(requestedSource.c_str()) != NULL) {
            resourcetype = DRCT;
            if (requestedSource[requestedSource.size() - 1] != '/') {
                response->setHeader("Location", uriplusslash);
                throw(301);
            }
        	return (requestedSource);
        }
        else if (access(requestedSource.c_str(), 0) == 0) {
            resourcetype = FILE;
        	return (requestedSource);
        }
    }
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