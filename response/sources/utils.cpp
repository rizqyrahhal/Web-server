/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 00:10:22 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/14 16:27:35 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"


bool checkIndexInsidDerctory(std::string *path) {
    std::string indexhtml = *path + "index.html"; //Just hardecode be to make it check for any index name
    std::string indexphp = *path + "index.php";
    // std::string indexpy = *path + "index.py";
    if (access(indexhtml.c_str(), 0) == 0) {
        *path = indexhtml;
        return true;
    }
    if (access(indexphp.c_str(), 0) == 0) {
        *path = indexphp;
        return true;
    }
    return false;
}

/* function to response body */
const std::string generatBody(std::string _requestedSource) {
    std::ifstream file(_requestedSource.c_str(), std::ifstream::binary);
    if (!file.is_open())
        throw(403);

    file.seekg(0, std::ios::end);
    int length = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(length);
    file.read(&buffer[0], length);
    file.close();

    return (std::string(buffer.begin(), buffer.end()));
}

int calculeBodySize(std::string _requestedSource) {
    std::ifstream file(_requestedSource.c_str(), std::ifstream::binary);
    if (!file.is_open())
        throw(403);

    file.seekg(0, std::ios::end);
    int length = file.tellg();
    file.seekg(0, std::ios::beg);
    
    file.close();
    return (length);
}










                    // else if (resp == 0)
                    // {
                    //     std::cout << "\n\n************************************************************ SWITCH TO RESPNSE PART ************************************************************\n";
                    //     Response response;
                    //     ResponseReturned res = response.CreatResponse(server, *client.request_client);
                    //     // std::cout << "isFile: " << res.getIsFile() << std::endl << "BodyFile: " << res.getBody() << std::endl;
                    //     //     std::cout << "\n***** Response ***** \n" << (res.getHeaders() + res.readfile()) << "\n----------------------------------\n";
                    //     // int sending = send(client.fd_client, (res.getHeaders()).c_str(), (res.getHeaders()).size(), 0);
                    //     std::string chunck = res.GetChanckFromResponse(255);
                    //     while(!chunck.empty()) {
                    //         int sending = send(client.fd_client, chunck.c_str(), chunck.size(), 0);
                    //         (void)sending;
                    //         // std::cout << "I SEND RESP TO THIS USER: " << client.fd_client << "\nSENDING: " <<  sending << std::endl;
                    //         chunck = res.GetChanckFromResponse(255);
                    //     }
                    //     std::cout << "\n###################################################################################################################################################\n\n";
                    //     //send correct response
                    //     sen = true; /// change with client_status_life
                    // }