/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePart.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 15:35:46 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/23 22:37:35 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"


size_t Response::_matchedLocationPosition;


Response::Response() {

}

std::string Response::CreatResponse(server server, request request) {
    // (void)servder;
    // (void)requdest;
        // Server server;
	    // Request request;
        Response response;
        try
        {
            /* First handling */
            _matchedLocation = response.GetMatchedLocationRequestUrl(server.locations, request.url);
            if (DEBUG) {
                std::cout << "_matchedLocationPosition: " << _matchedLocationPosition << "\n_matchedLocation: " << _matchedLocation << std::endl;
            }
            Response::IsLocationHaveRedirection(server.locations[_matchedLocationPosition]);
            Response::IsMethodAllowedInLocation(server.locations[_matchedLocationPosition].allow_methods, request.method);

            /*       Start Working On Method Type */ // for new it is OK but a don't know all casase to check

            /* Check Which Requested Method */
            if (request.method == "GET")
                response.GetMethod(server, request);
            // else if (request.getMethod() == "POST")
            //         response.PostMethod();
            // else if (request.getMethod() == "DELETE")
            //     response.DeleteMethod();
            else
                std::cout << "IF SHOWING THIS LINE IT IS A PROBLEME BEFORE WORKING ON THE REQUEST METHOD !!!!!!!!!\n";
        }
        catch(std::string response)
        {
            if(DEBUG) {
                std::cout << "in catch statment ================================================ \n";
            }
            // return the error page that throwen
            return (response);
        }
        // catch(generatedResponse response)
        // {
                /* if it is a response by body file return object contian header and fd_body and mumber function to read chunck by chunck from the body files */
        //     return (response);
        // }

		// ++++++++++++++++++++++++++++  body handler

        std::ifstream file("www/app/html/index.html", std::ifstream::binary);
        if (!file.is_open())
        {
        	std::cout << "Error in opening file\n";
        	std::exit(-404);
        }
        file.seekg(0, std::ios::end);
        int length = file.tellg();
        file.seekg(0, std::ios::beg);

        char buffer[length + 1];
        file.read(buffer, length);
		buffer[length] = '\0';

        file.close();
        // ############################


		// **********************

        // --------------- generate response
        HttpResponse new_response;

        new_response.setVersion("HTTP/1.1");
        new_response.setStatusCode(200);
        new_response.setStatusMessage("Ok"); // generat enume structe to geting statuse msg from its
		// new_response.setHeader("Date: ", generateDate());
        new_response.setHeader("Content-Type", "text/html"); //generateContentType()
        // new_response.setHeader("Content-Length", "0");
        // new_response.setHeader("Location", "/new_page");
        new_response.setHeader("Content-Length", std::to_string(length));
		new_response.setHeader("Conection", "close");  // request.getConection()
        new_response.setBody(buffer);
		// std::string response = new_response.generateResponse();
        // std::cout << "\n\n\n" << response << "\n\n\n";
        
	    // ******************


    // response from cgi
    
    return (new_response.generateResponse());
}

Response::~Response() {

}