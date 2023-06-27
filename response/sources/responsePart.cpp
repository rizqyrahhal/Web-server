/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePart.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 15:35:46 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/27 13:04:45 by rarahhal         ###   ########.fr       */
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
        // std::map<int, std::string> map_err_page;
        server.locations[0].index = "./www/app//html/index.html"; // need to addd in parssing !!!!
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
            // else if (request.getMethod() == "POST")
            //         response.PostMethod();
            else
                std::cout << "IF SHOWING THIS LINE IT IS A PROBLEME BEFORE WORKING ON THE REQUEST METHOD !!!!!!!!!\n";
        }
        catch(int statuscode) { //change int by short in futere
            // if (statuscode == 200){
                // std::cout << "***** Response OK ***** \n" << response.ResponseGeneratedFromStatusCode(statuscode, server, request) << "\n----------------------------------\n";
                return (response.ResponseGeneratedFromStatusCode(statuscode, server, request));
            // }
            // else{
                // std::cout << "***** Response OK ***** \n" << GenerateResponseFromStatusCode(statuscode, response) << "\n----------------------------------\n";
                // return (GenerateResponseFromStatusCode(statuscode, response));
            // }    
        }
        // catch(std::string response)
        // {
        //     /* catch the status code and then creat the response form construct a response and set the headers and evry where */
        //     #ifdef DEBUG
        //         std::cout << "in catch statment ================================================ \n" << response << std::endl;
        //     #endif
        //     // return the error page that throwen
        //     return (response);
        //     /* if it is a response by body file return object contian header and fd_body and mumber function to read chunck by chunck from the body files */
        //     // return (response);
        // }

    // response from cgi
    
    return ("ERROR *************************************\n");
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
	setHeader("Conection", "close"); // from map headers (request data)   request.getHeaderValue(std::string key)  <---- this function global to get any header from map headers in the request (key in this case = "Conection")

    /* HERE response have rederect message not have any body or something like this her is the probleme*/
	/* search about error page in map_error page */

    if (statuscode != 200)   // change this with categore redirection-error
    {
		// std::string err_page = SearchAboutErrorPage(statuscode, server.map_err_page);
		std::string err_page = SearchAboutErrorPageFormTowPlaces(statuscode, server.map_err_page, server.locations[_matchedLocationPosition].map_err_page); //the last map not implemented by parser
        if (!err_page.empty())
        	setBody(ReadErrorPage(err_page));
        else
            setBody(GenerateErrorPage(statuscode, getReason(statuscode)));
        setHeader("Content-Type", "text/html"); //generateContentType()
    }

    setHeader("Content-Length", std::to_string(getBodySize()));
    
	// GENERATE_THE_FINALE_RESPONSE();
    return generateResponse();
}


Response::~Response() {

}

// change from throw response to throw status code \|
// add the reading file  in vector \|
// swap between check rederection and check allowd methods, but need confinm with nginx

// 
// in GET flow files finished ---> in dyrctory l2an