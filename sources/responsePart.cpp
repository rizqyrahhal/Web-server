/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePart.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 15:35:46 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/21 20:00:28 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

Response::Response() {

}

Response::~Response() {

}

/* this function get the matched location position in the vectors location if axisting, 
   {!!!} but need more check if it is a location name without / in begin or something like that*/
size_t Response::GetMatchedLocationRequestUrl(std::vector<Locations> locations, std::string requesturi) {
    bool why = false;
    for(size_t i = 0; i < locations.size(); i++)
    {
        if (locations[i]._name.find(requesturi))
        {
            // size_t locationPositionInUri = requesturi.find(locations[i]._name);
            /* check if uri begin with location name */
            if (requesturi.find(locations[i]._name) == 0)
            {
                // std::cout << "Location: " << locations[i]._name << "\nURI: " << requesturi << std::endl;
                /* stor it to _matchedLocation if the length of its longer tnen last one */
                if (locations[i]._name.size() > _matchedLocation.size())
                {
                    why = true;
                    _matchedLocation = locations[i]._name;
                    _matchedLocationPosition = i;
                }
            }
        }
    }
    std::cout << "matchedLocationPosition: " << _matchedLocationPosition << "\nMatchedLocationStored: " << _matchedLocation << std::endl;
    if (why)
        return _matchedLocationPosition;
    throw (GenerateResponseFromStatusCode(404));
}

/* this function check if the matched location have redirection derictive 
    -> if it is handl it and return a response headers heve location header contian the first redirect path */
void IsLocationHaveRedirection(Locations matchedlocation) {
    std::cout << "Matched Location name: " << matchedlocation._name << std::endl;
    std::map<int, std::string>::iterator it = matchedlocation._redirect.begin();
    if (matchedlocation._redirect.size())
    {
        HttpResponse response;
        response.setHeader("Location", it->second);
        std::cout << "+_+_+_+_+_+_+_+_+_**************************\n";
        throw (GenerateResponseFromStatusCode(it->first, response));
    }
}


std::string CreatResponse() {
        Server server;
	    Request request;
        Response response;
        try
        {
            size_t MatchedLocation = response.GetMatchedLocationRequestUrl(server._locations, request.getUri());

            // std::cout << "++++++++_+_+_+_+_+_++_+_+_+_+_+_+_+_+_+_+_+_+_+_ "<< std::endl;

            IsLocationHaveRedirection(server._locations[MatchedLocation]);
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