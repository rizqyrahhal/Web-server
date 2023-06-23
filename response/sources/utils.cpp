/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:28:22 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/23 22:36:14 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"


std::string Response::GetResourceType(std::string requestedSource) {
    (void)requestedSource;
    // if ((requestedSource.size()-1) == )
    return ("");
}

std::string Response::GetRequestedSource(locations matchedlocation, std::string requesturi) {
    size_t position = requesturi.find_last_of("/");
    std::string checked, requestedSource;
    while (position != 0)
    {
        checked = requesturi.substr(position, requesturi.size());
        requestedSource = "." + matchedlocation.root + checked;
        /* check if exist */
        if (access(requestedSource.c_str(), 0) == 0)
        {
        	return (requestedSource);
        }
        else if (opendir(requestedSource.c_str()) != NULL)
        {
        	return (requestedSource);
        }
		position = requesturi.find_last_of("/", position - 1);
    }
    if (matchedlocation.name == requesturi)
    { 
        if (access(requestedSource.c_str(), 0) == 0)
        {
        	return (requestedSource);
        }
        else if (opendir(requestedSource.c_str()) != NULL)
        {
        	return (requestedSource);
        }
    }
    throw(GenerateResponseFromStatusCode(404));
}