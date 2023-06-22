/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:28:22 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/22 18:50:19 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

std::string GetRequestedSource(Locations matchedlocation, std::string requesturi) {
    // std::cout << "--- --- - - - - - - - " << matchedlocation._root << std::endl;

    size_t position = requesturi.find_last_of("/");
    while (position != 0)
    {
        std::string checked = requesturi.substr(position, requesturi.size());

        std::string requestedSource = "." + matchedlocation._root + checked;

        std::cout << "requestedSource: " << requestedSource << std::endl;

        std::cout << "the main uri :" << requestedSource << std::endl;

        /* check if exist */
        if (access(requestedSource.c_str(), 0) == 0)
        	return (requestedSource);

		position = requesturi.find_last_of("/", position - 1);

        // std::cout << "Position: " << position << std::endl;
    }
    throw(GenerateResponseFromStatusCode(404));
}

