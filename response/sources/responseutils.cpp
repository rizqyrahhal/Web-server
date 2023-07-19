/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseutils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rizqy <rizqy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:28:22 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/19 18:03:58 by rizqy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

void Response::checkForIndexFile(Response *response, server server, std::string &bodyfile, bool &isfile) {
    if (server.locations[response->_matchedLocationPosition].index.empty())
    {
        if (checkIndexInsidDerctory(&response->_requestedSource)) /* return bool and append index.html to requstedSource if existe */
            return; // this condition me be remove if not mandatory
        if (server.locations[response->_matchedLocationPosition].autoindex == "on")
        {

            /* generate outoindex */
            bodyfile = generateAutoindexFile(response->_requestedSource);
            isfile = false;
            throw(200);
        }
        else
            throw(403);
    }
    if (!server.locations[response->_matchedLocationPosition].index.empty()) // here need to ask what beffor sherch in derectory or in derective
    {
        response->_requestedSource = server.locations[_matchedLocationPosition].index;
        return ;
    }
}

bool Response::isCgi() {
    for (std::map<std::string, std::string>::iterator it = _cgi_mimeTypes.begin(); it != _cgi_mimeTypes.end(); it++) {
        if (_contentType == it->second)
            return true;
    }
    return false;
}

// ################################# adding 

std::pair<std::string, std::string> Response::_parseHeader( const std::string& line )  {
	std::string _key, _value;
	size_t _end = 0;

	std::string _line = trim(line);
	// key
	while (_end < _line.length() && _line[_end] != ':')	++_end;
	_key =  trim(_line.substr(0, _end));

	// value
	if (_end < _line.length())
		_value = trim(_line.substr(++_end));

	return std::make_pair(_key, _value);
}

void Response::parseCgiOutput( const std::string& cgioutput) {
	std::string _line;
	size_t _seek = 0;

    std::istringstream iss(cgioutput);

	while (std::getline(iss, _line)) {
		_seek += _line.length() + 1;
		_line = trim(_line);
		if (_line.empty()) break;
		std::pair<std::string, std::string> _header = _parseHeader(_line);
	    if (_header.first == "X_POWRED_PY:" || _header.first == "Status:") {
		    if (_header.first == "Status:")
			    setStatusCode(toNumber<int>(_header.second));
	    }
	    else
			setHeader(_header.first, _header.second);
	}

    if (!_status_code || _status_code == 200)
        _status_code == 200;
    else {
        throw(502);
    }

	if (cgioutput.size() <= _seek) {
		// cgioutput.clear();
		setHeader("Content-Lenth: ", "0");
		throw(_status_code);
	}

    bodyfile = std::string(cgioutput.begin() + _seek, cgioutput.end());
    isfile = false;
}



// adding ##################################







void Response::GetContentType(std::string requestedSource, std::map<std::string, std::string> mimetypes, std::string &contenttype) {
    contenttype = getMimeType(mimetypes, getFileExtantion(requestedSource));
}


/* this function check the resource existent and its type whit rediriction if the type directory and hav'not / in the end
 {!!!}new it is work but in simple casae  
 {!!!} case if have root /www/app and the url /www/app/html/ the source generate is /www/app/ ERROR the correct is /www/app/html/
 {!!!} this function need big change like getMatchedLocation */
std::string Response::GetRequestedSource(locations matchedlocation, std::string requesturi, bool &resourcetype, Response *response, std::string method) {
    size_t position = requesturi.find_last_of("/");
    std::string checked, requestedSource, uriplusslash;
    uriplusslash = requesturi + "/";

    #ifdef DEBUG
        std::cout << "matchedlocation.name: " << matchedlocation.name << std::endl;
        std::cout << "matchedlocation.root: " << matchedlocation.root << std::endl;
        std::cout << "requesturi: " << requesturi << std::endl;
        std::cout << "positionAFfterloop: " << position << std::endl << std::endl;
    #endif

        /* just hardcode to make "/" work as expected */
        if (matchedlocation.name == requesturi && requesturi == "/")
        {
            requestedSource = "." + matchedlocation.root;
            /* check if exist */
            if (opendir(requestedSource.c_str()) != NULL) {
                resourcetype = DRCT;
                // if (requestedSource[requestedSource.size() - 1] != '/') {
                //     if (method == "DELETE")
                //         throw (409);
                // response->setHeader("Location", uriplusslash);
                // throw(301);
                // }
        	    return (requestedSource);
            }
            else if (access(requestedSource.c_str(), 0) == 0) {
                resourcetype = FILE;
        	return (requestedSource);
            }
        }

        requestedSource = "." + requesturi;
            // requestedSource = "." + matchedlocation.root;
        if (opendir(requestedSource.c_str()) != NULL) {
            resourcetype = DRCT;
            if (requestedSource[requestedSource.size() - 1] != '/') {
                if (method == "DELETE")
                    throw (409);
                response->setHeader("Location", uriplusslash);
                throw(301);
            }
        	return (requestedSource);
        }
        else if (access(requestedSource.c_str(), 0) == 0) {
            resourcetype = FILE;
        	return (requestedSource);
        }

    while (position != 0)
    {
        checked = requesturi.substr(position, requesturi.size());
        requestedSource = "." + matchedlocation.root + checked;

        #ifdef DEBUG
        std::cout << "\n-------------------";
        std::cout << "positionINloop: " << position << std::endl << std::endl;
        std::cout << "checked: " << checked << "\n";
        std::cout << "requestedSource: " << requestedSource << "\n";
        std::cout << "uriplusslash: " << uriplusslash << "\n";
        std::cout << "********************************************************\n";
        #endif
    
        /* check if exist */
        if (opendir(requestedSource.c_str()) != NULL) {
            resourcetype = DRCT;
            if (requestedSource[requestedSource.size() - 1] != '/') {
                if (method == "DELETE")
                    throw (409);
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

                if (method == "DELETE")
                    throw (409);
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
/* in this above function thire is more and more hardcoding !!!! */




















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