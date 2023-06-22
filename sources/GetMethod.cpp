/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetMethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:24:42 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/22 19:03:18 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

            /*       Start Working On Method Type */ // for new it is OK but a don't know all casase to check



// void Response::PostMethod() {

// }

// void Response::DeleteMethod() {
    
// }

// std::string Response::CheckWhichRequestMethod(Server server, Request request) {
//     if (request.getMethod() == "GET")
//         Response::GetMethod(server, request);
//     // else if (request.getMethod() == "POST")
//     //     Response::PostMethod();
//     // else if (request.getMethod() == "DELETE")
//     //     Response::DeleteMethod();
//     else
//         std::cout << "IF SHOWING THIS LINE IT IS A PROBLEME BEFORE WORKING ON THE REQUEST METHOD !!!!!!!!!\n";
// }

void Response::GetMethod(Server server, Request request) {
    // (void)server;
    // (void)request;

    // !!!!!!!!!!!!!! STOP HERE but path need more and more handling !!!!!!!!!!!!!!!
    std::cout << "THE EXISTING PATH: " << GetRequestedSource(server._locations[Response::_matchedLocationPosition], request.getUri()) << std::endl;
}