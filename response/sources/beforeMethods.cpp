/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   beforeMethods.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araysse <araysse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:22:56 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/25 15:22:19 by araysse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

std::string Response::GetMatchedLocationRequestUrl(std::vector<locations> locations, std::string requesturi) {
    bool why = false;
    std::string matchedlocation;

    requesturi = removeReturnBack(requesturi); /* to remove ../ */

    for(size_t i = 0; i < locations.size(); i++)
    {
        if (locations[i].name == requesturi)
        {
            _matchedLocationPosition = i;
            return (locations[i].name);
        }
        if (locations[i].name.find(requesturi))
        {
            /* check if uri begin with location name */
            if (requesturi.find(locations[i].name) == 0)
            {
                /* stor it to _matchedLocation if the length of its longer tnen last one */
                if (locations[i].name.size() > matchedlocation.size())
                {
                    why = true;
                    matchedlocation = locations[i].name;
                    _matchedLocationPosition = i;
                }
            }
        }
    }
    if (why)
        return (matchedlocation);
    throw (404);
}

/* this function check if the matched location have redirection derictive 
    -> if it is handl it and return a response headers heve location header contian the first redirect path */
void Response::IsLocationHaveRedirection(locations matchedlocation, Response &response) {
    std::map<int, std::string>::iterator it = matchedlocation.redirect.begin();
    if (matchedlocation.redirect.size())
    {
        response.setHeader("Location", it->second);
        throw (it->first);
    }
}

void Response::IsMethodAllowedInLocation(std::vector<std::string> allowedmethod, std::string requestmethod, Response &response) {
    std::string methods;
    if (allowedmethod.size()) {   
        for(size_t i = 0; i < allowedmethod.size(); i++)
        {
            methods.append(allowedmethod[i]);
            methods.append(", ");
            if (allowedmethod[i] == requestmethod)
                return;
        }
        methods.erase(methods.find_last_of(","));
        response.setHeader("Allow", methods);
        throw (405);
    }
}