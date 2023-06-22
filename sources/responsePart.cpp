/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responsePart.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 15:35:46 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/22 01:05:49 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

Response::Response() {

}

Response::~Response() {

}

/* this function get the matched location position in the vectors location if axisting, 
   {!!!} but need more check if it is a location name without / in (begin/end) or something like that*/
size_t Response::GetMatchedLocationRequestUrl(std::vector<Locations> locations, std::string requesturi) {
    bool why = false;
    for(size_t i = 0; i < locations.size(); i++)
    {
        // std::cout << "matchedLocation: " << locations[i]._name << "\nURi: " << requesturi << std::endl;
        if (locations[i]._name == requesturi)
        {
            _matchedLocation = locations[i]._name;
            return (i);
        }
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
    // std::cout << "matchedLocationPosition: " << _matchedLocationPosition << "\nMatchedLocationStored: " << _matchedLocation << std::endl;
    if (why)
        return _matchedLocationPosition;
    throw (GenerateResponseFromStatusCode(404));
}

/* this function check if the matched location have redirection derictive 
    -> if it is handl it and return a response headers heve location header contian the first redirect path */
void Response::IsLocationHaveRedirection(Locations matchedlocation) {
    std::map<int, std::string>::iterator it = matchedlocation._redirect.begin();
    if (matchedlocation._redirect.size())
    {
        HttpResponse response;
        response.setHeader("Location", it->second);
        throw (GenerateResponseFromStatusCode(it->first, response));
    }
}

void Response::IsMethodAllowedInLocation(std::vector<std::string> allowedmethod, std::string requestmethod) {
    std::string methods;
    for(size_t i = 0; i < allowedmethod.size(); i++)
    {
        methods.append(allowedmethod[i]);
        methods.append(", ");
        if (allowedmethod[i] == requestmethod)
            return;
    }
    HttpResponse response;
    methods.erase(methods.find_last_of(","));
    response.setHeader("Allow", methods);
    throw (GenerateResponseFromStatusCode(405, response));
}

                                        /*       Start Working On Method Type */
std::string Response::GetRequestedSource(Locations matchedlocation, std::string requesturi) {
    // std::cout << "--- --- - - - - - - - " << matchedlocation._root << std::endl;

    size_t position = requesturi.find_last_of("/");
    std::string checked = requesturi.substr(position, requesturi.size());
    std::string requestedSource = matchedlocation._root + checked;

    // std::cout << "Position: " << position << std::endl;

    std::string path = "." + requestedSource;
    // std::cout << "requestedSource: " << path << std::endl;

    /* check if exist */

    if (access(path.c_str(), 0) == 0)
       return (path);

    throw(GenerateResponseFromStatusCode(404));
}

void Response::GetMethod(Server server, Request request) {
    // (void)server;
    // (void)request;

    // !!!!!!!!!!!!!! STOP HERE but path need more and more handling !!!!!!!!!!!!!!!
    std::cout << "THE EXISTING PATH: " << Response::GetRequestedSource(server._locations[Response::_matchedLocationPosition], request.getUri()) << std::endl;
}

// void Response::PostMethod() {

// }

// void Response::DeleteMethod() {
    
// }


void Response::CheckWhichRequestMethod(Server server, Request request) {
    if (request.getMethod() == "GET")
        Response::GetMethod(server, request);
    // else if (request.getMethod() == "POST")
    //     Response::PostMethod();
    // else if (request.getMethod() == "DELETE")
    //     Response::DeleteMethod();
    else
        std::cout << "IF SHOWING THIS LINE IT IS A PROBLEME BEFORE WORKING ON THE REQUEST METHOD !!!!!!!!!\n";
}

std::string CreatResponse() {
        Server server;
	    Request request;
        Response response;
        try
        {
            /* First handling */
            size_t MatchedLocation = response.GetMatchedLocationRequestUrl(server._locations, request.getUri());
            Response::IsLocationHaveRedirection(server._locations[MatchedLocation]);
            Response::IsMethodAllowedInLocation(server._locations[MatchedLocation]._allow_methods, request.getMethod());

            /* in the method place */
            Response::CheckWhichRequestMethod(server, request);
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
