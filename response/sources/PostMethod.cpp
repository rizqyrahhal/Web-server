/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 16:45:33 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/06 00:14:41 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"
bool supportUpload(locations matchedlocation) {
    if (!matchedlocation.upload_pass.empty())
        return true;
    return false;
}

void Response::PostMethod(server server, request request) {
    std::cout << "Upload_pass: " << server.locations[_matchedLocationPosition].upload_pass\
              << std::endl << "Requested Method: " << request.method << std::endl;
    if(supportUpload(server.locations[_matchedLocationPosition])) {
        // getFileName(); genarate or detect the filename
    }
    else {
        
    }
        
}
