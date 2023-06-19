/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePart.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 15:35:46 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/19 17:22:52 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

Response::Response() {

}
Response::~Response() {

}

std::string CreatResponse() {
    Server server;
	Request request;
    // Response response;

		// ---------------------  detecte status code and fill all needed data to generate response

		// ++++++++++++++++++++++++++++  body handler
        std::ifstream file("www/app/index.html", std::ifstream::binary);
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
        new_response.setHeader("Content-Length", std::to_string(length));
		// new_response.setHeader("Conection", request.getConection());
        new_response.setBody(buffer);
		// std::string response = new_response.generateResponse();
        // std::cout << "\n\n\n" << response << "\n\n\n";
        
	    // ******************


    // response from cgi
    
    return (new_response.generateResponse());
}