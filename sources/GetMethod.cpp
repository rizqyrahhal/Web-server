/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetMethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:24:42 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/22 22:51:57 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

            /*       Start Working On Method Type */ // for new it is OK but a don't know all casase to check



// void Response::PostMethod() {

// }

// void Response::DeleteMethod() {
    
// }

void Response::GetMethod(Server server, Request request) {
    // (void)server;
    // (void)request;

    // !!!!!!!!!!!!!! STOP HERE but path need more and more handling !!!!!!!!!!!!!!!
    _requestedSource = GetRequestedSource(server._locations[Response::_matchedLocationPosition], request.getUri());
    std::cout << "THE EXISTING PATH: " << GetRequestedSource(server._locations[Response::_matchedLocationPosition], request.getUri()) << std::endl;
}