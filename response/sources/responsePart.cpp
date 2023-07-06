/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePart.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 15:35:46 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/05 22:13:34 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"


size_t Response::_matchedLocationPosition;


Response::Response() {
    fillMimeTypes(_mimeTypes); // this data move to initial it in the beginig of programe one time to evit time delia in any response {!!!!!!!}
}

std::string Response::CreatResponse(server server, request request) {
    // (void)servder;
    // (void)requdest;
        // Server server;
	    // Request request;

        /* this data between this tow comment need to adding by parser */
        // parsing of config file need more check about the existing and permetion of any files or folder set inside its ...
        // std::map<int, std::string> map_err_page;
        
        // server.locations[0].index = "./www/app/html/index.html"; // need to addd in parssing !!!!
        
        /* this is the second comment : END OF COMMENT */
        
        Response response;
        try
        {
            /* First handling */


            // for (size_t i = 0; i < server.locations.size(); i++)
            // {
            //     std::cout << "locations.name[" << i << "] name: " << server.locations[i].root << std::endl;
            // }
            // std::cout << "URL: " << request.url << std::endl;

            _matchedLocation = response.GetMatchedLocationRequestUrl(server.locations, request.url);
            #ifdef DEBUG
                std::cout << "_matchedLocationPosition: " << _matchedLocationPosition << "\n_matchedLocation: " << _matchedLocation << std::endl;
                std::cout << "_matchedLocation.root: " << server.locations[_matchedLocationPosition].root << std::endl;
            #endif
            Response::IsMethodAllowedInLocation(server.locations[_matchedLocationPosition].allow_methods, request.method, response);
            Response::IsLocationHaveRedirection(server.locations[_matchedLocationPosition], response);

            /*       Start Working On Method Type */ // for new it is OK but a don't know all casase to check

            /* Check Which Requested Method */
            if (request.method == "GET")
                response.GetMethod(server, request);
            else if (request.method == "DELETE")
                response.DeleteMethod(server, request);
            else if (request.method == "POST")
                    response.PostMethod(server, request);
            else
                std::cout << "IF SHOWING THIS LINE IT IS A PROBLEME BEFORE WORKING ON THE REQUESTED METHOD !!!!!!!!!\n";
            throw(403); // just to be work !!!!!!!!!!!!!!
        }
        catch(int statuscode) { //change int by short in futere
                std::string *res = new std::string(response.ResponseGeneratedFromStatusCode(statuscode, server, request));
                // #ifdef DEBUG
                //     std::cout << "***** Response OK ***** \n" << res << "\n----------------------------------\n";
                // #endif
                return (*res); /* this res need to be inside the Response class to destroy it when finshed sending response to client */
        }

    // response from cgi
    return (NULL);
    // return ("ERROR *************************************\n");
}

/* rest small choise not handling in headers !!!!!!!!!!!!! */                // TRY  to merge this function with the abouve function in some sulution (my be make it a template function or something like this)
std::string Response::ResponseGeneratedFromStatusCode(int statuscode, server server, request request) {
	// request request;
	// Server	server;

    (void)request;
    /* Start line element */
    setVersion("HTTP/1.1");
	setStatusCode(statuscode);
	setStatusMessage(getReason(statuscode));

    /* set default headers */
	// setHeader("Date: ", generateDate());
	setHeader("Connection", "close"); // from map headers (request data)   request.getHeaderValue(std::string key)  <---- this function global to get any header from map headers in the request (key in this case = "Conection")

    /* HERE response have rederect message not have any body or something like this her is the probleme*/
	/* search about error page in map_error page */

    if (statuscode != 200 && statuscode != 201 && statuscode != 204)   // change this with categore redirection-error
    {
		// std::string err_page = SearchAboutErrorPage(statuscode, server.map_err_page);
		std::string err_page = SearchAboutErrorPageFormTowPlaces(statuscode, server.map_err_page, server.locations[_matchedLocationPosition].map_err_page); //the last map not implemented by parser
        if (!err_page.empty())
        	setBody(ReadErrorPage(err_page));
        else
            setBody(GenerateErrorPage(statuscode, getReason(statuscode)));
        setHeader("Content-Type", "text/html"); //generateContentType(), that is oky for new becous the error type in all time html and the other way detecte and assigne in the same place
    }

    /* adding headers */
    if (statuscode != 201 && statuscode != 204)
        setHeader("Content-Length", std::to_string(getBodySize()));
    
	// GENERATE_THE_FINALE_RESPONSE();
    std::string res = generateResponse();
    
    return res;
}

Response::~Response() {

}

// change from throw response to throw status code \|
// add the reading file  in vector \|
// swap between check rederection and check allowd methods, but need confinm with nginx








/* new in DELETE METHOD handl file and directory 409 rest 403 and 500 and cgi for both file and directory */


