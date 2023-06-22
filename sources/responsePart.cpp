/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePart.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 15:35:46 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/22 19:05:41 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

Response::Response() {

}

std::string Response::CreatResponse() {
        Server server;
	    Request request;
        Response response;
        try
        {
            /* First handling */
            size_t MatchedLocation = response.GetMatchedLocationRequestUrl(server._locations, request.getUri());
            Response::IsLocationHaveRedirection(server._locations[MatchedLocation]);
            Response::IsMethodAllowedInLocation(server._locations[MatchedLocation]._allow_methods, request.getMethod());

            /* Check Which Requested Method */
            if (request.getMethod() == "GET")
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
            std::cout << "in catch statment ================================================ \n";
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

        new_response.setVersion(request.getVersion());
        new_response.setStatusCode(200);
        new_response.setStatusMessage("Ok"); // generat enume structe to geting statuse msg from its
		// new_response.setHeader("Date: ", generateDate());
        new_response.setHeader("Content-Type", "text/html"); //generateContentType()
        // new_response.setHeader("Content-Length", "0");
        // new_response.setHeader("Location", "/new_page");
        new_response.setHeader("Content-Length", std::to_string(length));
		new_response.setHeader("Conection", request.getConection());
        new_response.setBody(buffer);
		// std::string response = new_response.generateResponse();
        // std::cout << "\n\n\n" << response << "\n\n\n";
        
	    // ******************


    // response from cgi
    
    return (new_response.generateResponse());
}

Response::~Response() {

}