/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePart.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 15:35:46 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/25 01:53:56 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

size_t Response::_matchedLocationPosition;

Response::Response() {
    _envp = NULL;
    _argv = NULL;
	fillMimeTypes(_cgi_mimeTypes);
    _mimeTypes = readMimeTypes("./template/mime.types");
}

ResponseReturned Response::CreatResponse(server server, request request) {
    Response response;
    try
    {
        _matchedLocation = response.GetMatchedLocationRequestUrl(server.locations, request.url);
        
        #ifdef DEBUG
            std::cout << "_matchedLocationPosition: " << _matchedLocationPosition << "\n_matchedLocation: " << _matchedLocation << std::endl;
            std::cout << "_matchedLocation.root: " << server.locations[_matchedLocationPosition].root << std::endl;
            std::cout << "Method: " << request.method << std::endl;
        #endif
        Response::IsMethodAllowedInLocation(server.locations[_matchedLocationPosition].allow_methods, request.method, response);
        Response::IsLocationHaveRedirection(server.locations[_matchedLocationPosition], response);

        /* Check Which Requested Method */
        if (request.method == "GET")
            response.GetMethod(server, request, bodyfile, isfile);
        else if (request.method == "DELETE")
            response.DeleteMethod(server, request);
        else if (request.method == "POST")
                response.PostMethod(server, request);
        else
            std::cout << "IF SHOWING THIS LINE IT IS A PROBLEME BEFORE WORKING ON THE REQUESTED METHOD !!!!!!!!!\n";
    }
    catch(int statuscode) {
        std::string *res = new std::string(response.ResponseGeneratedFromStatusCode(statuscode, server, request, bodyfile, isfile));
        ResponseReturned resp;
        resp.setHeaders(*res);
        if (!bodyfile.empty()) {
            resp.setBody(bodyfile);
            resp.setIsFile(isfile);
        }
        delete res;
        remove(request.bodyfile_name.c_str());
            
        #ifdef DEBUG
            std::cout << "\n***** Response ***** \n" << (resp.getHeaders() + resp.readfile()) << "\n----------------------------------\n";
        #endif
        
        return (resp);
    }

    /* here just to evet the return value error */
    ResponseReturned resp;
    return (resp);
}

void Response::cgi(server server, request request, Response &response) {
	Cgi cgi;
    int file = 0;

    #ifdef CGI_DEBUG
        std::cout << "########## Cgi MAP ###########" << std::endl; /// when set this to matche given an error
        for (std::map<std::string, std::string>::iterator it = server.locations[6].cgi.begin(); it != server.locations[6].cgi.end(); it++) {
            std::cout << it->first << "=" << it->second << std::endl;
        }   
        std::cout << "########## :) ###########" << std::endl << std::endl;
    #endif

    _cgiBinPath = cgi.getCgiPath(server.locations[_matchedLocationPosition].cgi, _contentType);  
    
    #ifdef CGI_DEBUG
       std::cout << "_cgiBinPath: " << _cgiBinPath << std::endl;
    #endif
    
	cgi.fillEnvp(request, server, _requestedSource, _contentType);
    cgi.fillArgv(_cgiBinPath, _requestedSource);
    _envp = cgi.vectorToCharArray(cgi._envp);
    _argv = cgi.vectorToCharArray(cgi._argv);
    if (request.method == "GET") {
        file = open(_requestedSource.c_str(), 666);
    }
    else if (request.method == "POST") {
        file = request.bodyFile;
    }

    response.parseCgiOutput(cgi.execut(_argv[0], _argv, _envp, file));

    close(file);
    throw(200);
}


std::string Response::ResponseGeneratedFromStatusCode(int statuscode, server server, request request, std::string &bodyfile, bool &isfile) {
    (void)request;
    /* Start line element */
    setVersion("HTTP/1.1");
	setStatusCode(statuscode);
	setStatusMessage(getReason(statuscode));

    /* set default headers */
	setHeader("Date", getCurrentDate());
    setHeader("Server", server._name);

	setHeader("Connection", "close");

    if (statuscode != 200 && statuscode != 201 && statuscode != 204)
    {
		std::string err_page = SearchAboutErrorPageFormTowPlaces(statuscode, server.map_err_page, server.locations[_matchedLocationPosition].map_err_page);
        if (!err_page.empty()) {
            setHeader("Content-Length", std::to_string(calculeBodySize(err_page)));
		    bodyfile = err_page;
            isfile = true;
        }
        else {
            bodyfile = GenerateErrorPage(statuscode, getReason(statuscode));
            setHeader("Content-Length", std::to_string(bodyfile.size()));
            isfile = false;
        }
        setHeader("Content-Type", "text/html");
    }

    /* adding headers */
    if (!isfile && statuscode == 200 && getBodySize()) {    
        setHeader("Content-Length", std::to_string(getBodySize()));
    }
    else if (!isfile && statuscode == 200 && bodyfile.size())
        setHeader("Content-Length", std::to_string(bodyfile.size()));
        

	/* GENERATE_THE_FINALE_RESPONSE */
    std::string res = generateResponse();

    return res;
}

Response::~Response() {
    if (_envp != NULL && _argv != NULL){
        delete[] _envp;
        delete[] _argv;
    }
}
