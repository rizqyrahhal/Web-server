/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePart.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 15:35:46 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/21 18:08:14 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

size_t Response::_matchedLocationPosition;

Response::Response() {
    _envp = NULL;
    _argv = NULL;
    _cgi_mimeTypes = readMimeTypes("./template/cgi-mime.types");
    _mimeTypes = readMimeTypes("./template/mime.types");  //this data move to initial it in the beginig of programe one time to evit time delia in any response {!!!!!!!}
}

ResponseReturned Response::CreatResponse(server server, request request) {
    std::cout << "LOcation[0]  : allowedmethod size : " << server.locations[0].allow_methods.size() << std::endl;
    std::cout << "THE ROOOT IN LOCATION : " << server.locations[0].root << std::endl; 
    // std::cout << "########## eenrtrer ###########" << std::endl;
    // for (std::map<std::string, std::string>::iterator it = request.map_request.begin(); it != request.map_request.end(); it++) {
    //     std::cout << it->first << "=" << it->second << std::endl;
    //     // it->second.erase(0, 1);
    // }
    // std::cout << "########## ssorteee ###########" << std::endl << std::endl;

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
            // if (!isfile && statuscode == 200)
            //     setHeader("Content-Length", std::to_string(bodyfile.size()));
            delete res;
            // if (resp.getHeaders().empty())
            //     std::cout << "\n***** Response ***** \n" << (resp.readfile()) << "\n----------------------------------\n";
            // else
            std::cout << "\n***** Response ***** \n" << (resp.getHeaders() + resp.readfile()) << "\n----------------------------------\n";  /////////////
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
    // char **envp = cgi.vectorToCharArray(cgi._envp);
    // char **argv = cgi.vectorToCharArray(cgi._argv);
    _envp = cgi.vectorToCharArray(cgi._envp);
    _argv = cgi.vectorToCharArray(cgi._argv);
    if (request.method == "GET") {
        file = open(_requestedSource.c_str(), 666);
    }
    else if (request.method == "POST") {
        // std::cout << "IN POST CGI CONDITION THIS IS THE FILE DESCRIPTOR NUMBER: " << request.bodyFile << std::endl;
        file = request.bodyFile;
    }

    response.parseCgiOutput(cgi.execut(_argv[0], _argv, _envp, _requestedSource, file));

    // delete[] envp;
    // delete[] argv;
    close(file); // need to check if close remove this line
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
    setHeader("Server", server.server_name);

	// setHeader("Connection", searchInRequestedHeader(request.map_request, "Connection"));
	setHeader("Connection", "close"); // from map headers (request data) use the line above when the request.map_request fixed

    if (statuscode != 200 && statuscode != 201 && statuscode != 204)
    {
		std::string err_page = SearchAboutErrorPageFormTowPlaces(statuscode, server.map_err_page, server.locations[_matchedLocationPosition].map_err_page); //the last map not implemented by parser
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

    /* adding headers */      //////////////////   have a small problem with content-length 
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
    // _requestedSource.clear();
}

/* the WORK rest :

    WHEN THE MEERG WORK AS NEED  REPLACE THE NETWORKIN FOLDER BY THE SAME ONE IN THE meerg REPO (THIS METHOD OF WORK BE ALL TIME OF WORKIN ON WEBSERV) 



    function to creat tomporare file name

    
    the index.php and index.by   in dyrectory // not mandatory just a *hawas* 

    matched location and matched source need work blkhosos source


    make more test in the matched location and the searching about requested source 


    
    ** the write PIPE error from the accepet neeed SIGNAL TO egnor it



    ** the 304 and 201 test it before push




    mal server_name 3la achmen server matched taydwi abdl3alim wo ana aslan tanakhod one server and one request

*/


// while true; do leaks webserv; sleep 1 ; done;





/*
   + problem in allow_methods directive insinde the location (this problem showen in test with one location ) {
    THE TEST CASE :
            location /www {
        root ./www/app
        allow_methods GET POST DELETE
        autoindex on
    }
   }

    +  THE same above problem with the root directive
*/





























		// char* envp[] = {
		// (char*)"SERVER_PROTOCOL=HTTP/1.1",
		// (char*)"REQUEST_METHOD=GET",
		// (char*)"PATH_INFO=/Users/rarahhal/Desktop/meerge/tools/cgi-scripts/php/hello.php",
		// (char*)"REDIRECT_STATUS=200",
        // (char*)"SERVER_PORT=8080",
		// (char*)"AUTH_TYPE",
		// (char*)"CONTENT_LENGTH",
        // (char*)"CONTENT_TYPE=application/x-httpd-php",
		// (char*)"GATEWAY_INTERFACE",
		// (char*)"PATH_TRANSLATED",
        // (char*)"QUERY_STRING",
		// (char*)"REMOTE_ADDR",
        // (char*)"REMOTE_HOST",
		// (char*)"REMOTE_IDENT",
        // (char*)"REMOTE_USER",
        // (char*)"SCRIPT_NAME=/Users/rarahhal/Desktop/meerge/tools/cgi-scripts/php/hello.php",
        // (char*)"SCRIPT_FILENAME=/Users/rarahhal/Desktop/meerge/tools/cgi-scripts/php/hello.php",
		// (char*)"SERVER_NAME",
        // (char*)"SERVER_SOFTWARE=CGI/1.1",
		// // (char*)"Accept-Encoding=gzip, deflate, br",
		// // (char*)"Accept-Language=en-US,en;q=0.9",
		// // (char*)"Cache-Control=max-age=0",
		// // (char*)"Connection=keep-alive",
		// // (char*)"Cookie=_ga=GA1.1.1276071924.1689035207; _ga_BVS1XM9LYJ=GS1.1.1689035207.1.1.1689035438.8.0.0; __gads=ID=d57167e481e2e578-223635ec3ae00050:T=1689035229:RT=1689036300:S=ALNI_MZeZpUb6CJn5WXJvh1UhjY-QILGAw; __gpi=UID=00000c65db6194a3:T=1689035229:RT=1689036300:S=ALNI_MZg5-InE8f8kHbBfHhC8yHKkIfFeg; PHPSESSID=a6dd5c948bdbfebf6dc11f830e45afb3",
		// // (char*)"Host=localhost:8080",
		// // (char*)"Sec-Fetch-Dest=document",
		// // (char*)"Sec-Fetch-Mode=navigate",
		// // (char*)"Sec-Fetch-Site=none",
		// // (char*)"Sec-Fetch-User=?1",
		// // (char*)"Upgrade-Insecure-Requests=1",
		// // (char*)"User-Agent=Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36",
		// // (char*)"sec-ch-ua='Not.A/Brand';v='8', 'Chromium';v='114', 'Google Chrome';v='114'",
		// // (char*)"sec-ch-ua-mobile=?0",
		// // (char*)"sec-ch-ua-platform='macOS'",
		// // (char*)"Accept=text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
		// // (char*)"",
		// // (char*)"",
		// nullptr
		// };