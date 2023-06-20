/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePart.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 15:35:46 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/20 22:38:18 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

Response::Response() {

}

Response::~Response() {

}

// void CheckMethod() {
    
// }

// void CheckUrl() {
    
// }

int Response::GetMatchedLocationRequestUrl(std::vector<Locations> locations, std::string requesturi) {
    for(unsigned int i = 0; i < locations.size(); i++)
    {
        if (locations[i]._name.find(requesturi) == 0)
        {
            
            std::cout << "iiiiiiiiiii: " << i << std::endl;
        }
        return i;
    }
    throw (GenerateResponseFromStatusCode(404));
}

void IsLocationHaveRedirection(Locations matchedlocation) {
    std::cout << "Matched Location name: " << matchedlocation._name << std::endl;       //// working here 
}

std::string CreatResponse() {
        Server server;
	    Request request;
        Response response;
        try
        {
            int MatchedLocation = response.GetMatchedLocationRequestUrl(server._locations, request.getUri());
            // std::cout << "++++++++_+_+_+_+_+_++_+_+_+_+_+_+_+_+_+_+_+_+_+_ "<< std::endl;
            IsLocationHaveRedirection(server._locations[MatchedLocation]);
        }
        catch(std::string response)
        {
            std::cout << "in catch statment ================================================ \n";
            return (response);
            // return the error page  that throwen
        }

        // CheckMethod(); // to undarstand the way need to take it
        // CheckUrl(); //  check url and the allowd method in location

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