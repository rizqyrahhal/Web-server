/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseutils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:28:22 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/22 20:00:35 by rarahhal         ###   ########.fr       */
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

void Response::parseCgiOutput( const std::string& cgioutput) {
	std::string _line;
	size_t _seek = 0;

    if (cgioutput.empty())
        throw(502);

    std::istringstream iss(cgioutput);

	while (std::getline(iss, _line)) {
		_seek += _line.length() + 1;
		_line = trim(_line);
		if (_line.size() <= 1)
            break;
		std::pair<std::string, std::string> _header = parseHeader(_line);
	    if (_header.first == "X-Powered-By" || _header.first == "Status") {
		    if (_header.first == "Status")
			    setStatusCode(toNumber<int>(_header.second));
	    }
        else if (_header.first == "Content-type")
			_header.first  = "Content-Type";
	    else
			setHeader(_header.first, _header.second);
	}

    if (!_status_code || _status_code == 200)
        _status_code = 200;
    else {
        throw(502);
    }

	if (cgioutput.size() <= _seek) {
		setHeader("Content-Length", "0");
		throw(_status_code);
	}

    setBody(std::string(cgioutput.begin() + _seek, cgioutput.end()));
    isfile = false;
}

void Response::GetContentType(std::string requestedSource, std::map<std::string, std::string> mimetypes, std::string &contenttype) {
    contenttype = getMimeType(mimetypes, getFileExtantion(requestedSource));
    if (contenttype.empty())
        contenttype = "text/plain";
}

//   ../../../             ../   be like that / 
std::string Response::GetRequestedSource(locations matchedlocation, std::string requesturi, bool &resourcetype, Response *response, std::string method) {
    // std::cout << "REQUESTED uri: " << requesturi << std::endl << std::endl;

    DIR *dir;
	std::string root = matchedlocation.root;
    std::string uriplusslash = requesturi + "/";
	size_t position;
	std::string requestedSource;
	


    // std::cout << "########## ENTREE INSIDE THE LOOP\n";
    while (root.size())
    {
		requestedSource = root + requesturi;
		// std::cout << "ROOT + URI : " << requestedSource << std::endl;
        if (requesturi == root || requesturi == "/")
        {
            /* check if exist */
            dir = opendir(requestedSource.c_str());
            if (dir != NULL) {
                resourcetype = DRCT;
                // if (requestedSource[requestedSource.size() - 1] != '/') {
                //     if (method == "DELETE")
                //         throw (409);
                // response->setHeader("Location", uriplusslash);
                // throw(301);
                // }
                closedir(dir);
        	    return (requestedSource);
            }
            else if (access(requestedSource.c_str(), 0) == 0) {
                resourcetype = FILE;
        	    return (requestedSource);
            }
        }
		
        /* check if exist */
        dir = opendir(requestedSource.c_str());
        if (dir != NULL) {
            resourcetype = DRCT;
            if (requestedSource[requestedSource.size() - 1] != '/') {
                if (method == "DELETE") {
                    closedir(dir);
                    throw (409);
                }
                response->setHeader("Location", uriplusslash);
                closedir(dir);
                throw(301);
            }
            closedir(dir);
        	return (requestedSource);
        }
        else if (access(requestedSource.c_str(), 0) == 0) {
            resourcetype = FILE;
        	return (requestedSource);
        }

		position = root.find_last_of("/");
    // std::cout << "positionINloop: " << position << std::endl << std::endl;
		if (position == std::string::npos)
   			throw(404);	
		root.erase(position, root.size());
		// std::cout << "ROOT NEW: " << root << std::endl;
    }
    // std::cout << "########## GO OUT THE LOOP\n";
    std::cout << "requestedSource: " << requestedSource << "\n";
    throw(404);
}



/* this function check the resource existent and its type whit rediriction if the type directory and hav'not / in the end
 {!!!}new it is work but in simple casae  
 {!!!} case if have root /www/app and the url /www/app/html/ the source generate is /www/app/ ERROR the correct is /www/app/html/
 {!!!} this function need big change like getMatchedLocation */
// std::string Response::GetRequestedSource(locations matchedlocation, std::string requesturi, bool &resourcetype, Response *response, std::string method) {   // need to close the oppening directory and optemese the logic
//     size_t position = requesturi.find_last_of("/");
//     std::string checked, requestedSource, uriplusslash;
//     uriplusslash = requesturi + "/";
//     DIR *dir;

//     #ifdef DEBUG
//         std::cout << "matchedlocation.name: " << matchedlocation.name << std::endl;
//         std::cout << "matchedlocation.root: " << matchedlocation.root << std::endl;
//         std::cout << "requesturi: " << requesturi << std::endl;
//         std::cout << "positionAFfterloop: " << position << std::endl << std::endl;
//     #endif

//         /* just hardcode to make "/" work as expected */
//         if (matchedlocation.name == requesturi && requesturi == "/")
//         {
//             requestedSource = matchedlocation.root;
//             /* check if exist */
//             dir = opendir(requestedSource.c_str());
//             if (dir != NULL) {
//                 resourcetype = DRCT;
//                 // if (requestedSource[requestedSource.size() - 1] != '/') {
//                 //     if (method == "DELETE")
//                 //         throw (409);
//                 // response->setHeader("Location", uriplusslash);
//                 // throw(301);
//                 // }
//                 closedir(dir);
//         	    return (requestedSource);
//             }
//             else if (access(requestedSource.c_str(), 0) == 0) {
//                 resourcetype = FILE;
//         	    return (requestedSource);
//             }
//         }

//         requestedSource = requesturi;
//             // requestedSource = "." + matchedlocation.root;

//         dir = opendir(requestedSource.c_str());
//         if (dir != NULL) {
//             resourcetype = DRCT;
//             if (requestedSource[requestedSource.size() - 1] != '/') {
//                 if (method == "DELETE"){
//                     closedir(dir);
//                     throw (409);
//                 }
//                 response->setHeader("Location", uriplusslash);
//                 closedir(dir);
//                 throw(301);
//             }
//             closedir(dir);
//         	return (requestedSource);
//         }
//         else if (access(requestedSource.c_str(), 0) == 0) {
//             resourcetype = FILE;
//         	return (requestedSource);
//         }

//     while (position != 0)
//     {
//         checked = requesturi.substr(position, requesturi.size());
//         requestedSource = matchedlocation.root + checked;

//         #ifdef DEBUG
//         std::cout << "\n-------------------";
//         std::cout << "positionINloop: " << position << std::endl << std::endl;
//         std::cout << "checked: " << checked << "\n";
//         std::cout << "requestedSource: " << requestedSource << "\n";
//         std::cout << "uriplusslash: " << uriplusslash << "\n";
//         std::cout << "********************************************************\n";
//         #endif
    
//         /* check if exist */
//         dir = opendir(requestedSource.c_str());
//         if (dir != NULL) {
//             resourcetype = DRCT;
//             if (requestedSource[requestedSource.size() - 1] != '/') {
//                 if (method == "DELETE") {
//                     closedir(dir);
//                     throw (409);
//                 }
//                 response->setHeader("Location", uriplusslash);
//                 closedir(dir);
//                 throw(301);
//             }
//             closedir(dir);
//         	return (requestedSource);
//         }
//         else if (access(requestedSource.c_str(), 0) == 0) {
//             resourcetype = FILE;
//         	return (requestedSource);
//         }
// 		position = requesturi.find_last_of("/", position - 1);
//     }
//     if (matchedlocation.root == requesturi || matchedlocation.name == requesturi)
//     {
    
//         requestedSource = requesturi;
//         /* check if exist */
//         dir = opendir(requestedSource.c_str());
//         if (dir != NULL) {
//             resourcetype = DRCT;
//             if (requestedSource[requestedSource.size() - 1] != '/') {
//                 if (method == "DELETE") {
//                     closedir(dir);
//                     throw (409);
//                 }
//                 response->setHeader("Location", uriplusslash);
//                 closedir(dir);
//                 throw(301);
//             }
//             closedir(dir);
//         	return (requestedSource);
//         }
//         else if (access(requestedSource.c_str(), 0) == 0) {
//             resourcetype = FILE;
//         	return (requestedSource);
//         }
//     }
//     // if (requesturi == "/tools/cgi-test/setCookie.py") {
//     //     resourcetype = FILE;
//     //     return ("/Users/rarahhal/Desktop/network_branch/tools/cgi-test/setCookie.py");
//     // }
    
//     throw(404);
// }
/* in this above function thire is more and more hardcoding !!!! */



// std::string Response::GetRequestedSource(locations matchedlocation, std::string requesturi, bool &resourcetype, Response *response, std::string method) {   // need to close the oppening directory and optemese the logic
//     size_t position = requesturi.find_last_of("/");
//     std::string checked, requestedSource, uriplusslash;
//     uriplusslash = requesturi + "/";
//     DIR *dir;

//     #ifdef DEBUG
//         std::cout << "matchedlocation.name: " << matchedlocation.name << std::endl;
//         std::cout << "matchedlocation.root: " << matchedlocation.root << std::endl;
//         std::cout << "requesturi: " << requesturi << std::endl;
//         std::cout << "positionAFfterloop: " << position << std::endl << std::endl;
//     #endif

//         /* just hardcode to make "/" work as expected */
//         if (matchedlocation.name == requesturi && requesturi == "/")
//         {
//             requestedSource = "." + matchedlocation.root;
//             /* check if exist */
//             dir = opendir(requestedSource.c_str());
//             if (dir != NULL) {
//                 resourcetype = DRCT;
//                 // if (requestedSource[requestedSource.size() - 1] != '/') {
//                 //     if (method == "DELETE")
//                 //         throw (409);
//                 // response->setHeader("Location", uriplusslash);
//                 // throw(301);
//                 // }
//                 closedir(dir);
//         	    return (requestedSource);
//             }
//             else if (access(requestedSource.c_str(), 0) == 0) {
//                 resourcetype = FILE;
//         	    return (requestedSource);
//             }
//         }

//         requestedSource = "." + requesturi;
//             // requestedSource = "." + matchedlocation.root;

//         dir = opendir(requestedSource.c_str());
//         if (dir != NULL) {
//             resourcetype = DRCT;
//             if (requestedSource[requestedSource.size() - 1] != '/') {
//                 if (method == "DELETE"){
//                     closedir(dir);
//                     throw (409);
//                 }
//                 response->setHeader("Location", uriplusslash);
//                 closedir(dir);
//                 throw(301);
//             }
//             closedir(dir);
//         	return (requestedSource);
//         }
//         else if (access(requestedSource.c_str(), 0) == 0) {
//             resourcetype = FILE;
//         	return (requestedSource);
//         }

//     while (position != 0)
//     {
//         checked = requesturi.substr(position, requesturi.size());
//         requestedSource = "." + matchedlocation.root + checked;

//         #ifdef DEBUG
//         std::cout << "\n-------------------";
//         std::cout << "positionINloop: " << position << std::endl << std::endl;
//         std::cout << "checked: " << checked << "\n";
//         std::cout << "requestedSource: " << requestedSource << "\n";
//         std::cout << "uriplusslash: " << uriplusslash << "\n";
//         std::cout << "********************************************************\n";
//         #endif
    
//         /* check if exist */
//         dir = opendir(requestedSource.c_str());
//         if (dir != NULL) {
//             resourcetype = DRCT;
//             if (requestedSource[requestedSource.size() - 1] != '/') {
//                 if (method == "DELETE") {
//                     closedir(dir);
//                     throw (409);
//                 }
//                 response->setHeader("Location", uriplusslash);
//                 closedir(dir);
//                 throw(301);
//             }
//             closedir(dir);
//         	return (requestedSource);
//         }
//         else if (access(requestedSource.c_str(), 0) == 0) {
//             resourcetype = FILE;
//         	return (requestedSource);
//         }
// 		position = requesturi.find_last_of("/", position - 1);
//     }
//     if (matchedlocation.root == requesturi || matchedlocation.name == requesturi)
//     {
    
//         requestedSource = "." + requesturi;
//         /* check if exist */
//         dir = opendir(requestedSource.c_str());
//         if (dir != NULL) {
//             resourcetype = DRCT;
//             if (requestedSource[requestedSource.size() - 1] != '/') {
//                 if (method == "DELETE") {
//                     closedir(dir);
//                     throw (409);
//                 }
//                 response->setHeader("Location", uriplusslash);
//                 closedir(dir);
//                 throw(301);
//             }
//             closedir(dir);
//         	return (requestedSource);
//         }
//         else if (access(requestedSource.c_str(), 0) == 0) {
//             resourcetype = FILE;
//         	return (requestedSource);
//         }
//     }
//     // if (requesturi == "/tools/cgi-test/setCookie.py") {
//     //     resourcetype = FILE;
//     //     return ("/Users/rarahhal/Desktop/network_branch/tools/cgi-test/setCookie.py");
//     // }
    
//     throw(404);
// }























// void Response::parseCgiOutput( const std::string& cgioutput) {
// 	std::string _line;
// 	size_t _seek = 0;

//     if (cgioutput.empty())
//         throw(502);

//     std::istringstream iss(cgioutput);

// 	while (std::getline(iss, _line)) {
// 		_seek += _line.length() + 1;
// 		_line = trim(_line);
// 		if (_line.size() <= 1)
//             break;
// 		std::pair<std::string, std::string> _header = parseHeader(_line);
// 	    if (_header.first == "X-Powered-By" || _header.first == "Status") {
// 		    if (_header.first == "Status")
// 			    setStatusCode(toNumber<int>(_header.second));
// 	    }
// 	    else{
//                 if (_header.first.find("HTTP/1.1") != std::string::npos) {
//                 // std::cout << "CGI HEADER : " <<  _header.first << std::endl;
//                     // std::cout << "HERERE ::   " << std::string(_header.first.begin() + 9, _header.first.begin() + 12);
//                     setStatusCode(toNumber<int>(std::string(_header.first.begin() + 9, _header.first.begin() + 12)));
//                     // setStatusCode(getStatusCodeFromCgi(_header.first));
//                 }
//                 // else if (_header.first == "Content-Type") {
// 			    //     setHeader(_header.first, "text/plain");
//                 // }
//                 else
//                 {
//                     std::cout << "00-------------------------------------   " << _header.first << std::endl;

// 			        setHeader(_header.first, _header.second);
//                 }
//             // else
//         }
// 	}
















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