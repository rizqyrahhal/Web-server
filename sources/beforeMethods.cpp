/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   beforeMethods.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:22:56 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/22 15:26:32 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"


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