/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 11:09:55 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/15 00:07:39 by rarahhal         ###   ########.fr       */
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

void Response::DeleteMethod(server server, request request) { /* Delete Metthod dosn't support Cgi */
    _requestedSource = Response::GetRequestedSource(server.locations[Response::_matchedLocationPosition], request.url, _resourceType, &(*this), request.method);
    #ifdef DEBUG
        std::cout << "_requestedSource: " << _requestedSource << std::endl;
        std::cout << "_resourceType: ";
        if(_resourceType)
            std::cout << "FILE\n";
        else
            std::cout << "DERCTORY\n";
    #endif
	/* directory handling */
	if (_resourceType == DRCT) {
        // if (!server.locations[_matchedLocationPosition].cgi.empty()) { /* CGI */
        //     if (!checkIndexInsidDerctory(&_requestedSource))  // here be to chenge to check index.php or index.by becouse its cgi !!!!!!
        //         throw(403);
        //     /* run cgi on requested file with DELETE REQUEST_METHOD && then Return the code depending on CGI -->  doing that in cgi condition beloow */
        // }
		// else if (deleteDirectory(_requestedSource))
		// 	throw(204);
		if (deleteDirectory(_requestedSource))
			throw(204);
    }
    Response::GetContentType(_requestedSource, _mimeTypes, _contentType);
    #ifdef DEBUG
        std::cout << "contentType : " << _contentType << std::endl;
    #endif
    /* CGI */
    // if (!server.locations[_matchedLocationPosition].cgi.empty())
    // {
    //     std::cout << ":;::: -_-------------------------------------- IN CGI -----------";
    // /// working here !!!!!!!!!!! //////
	// 	cgi(server, request);
    // }

    /* file handling */
	if (std::remove(_requestedSource.c_str()) == 0)
		throw (204);
}