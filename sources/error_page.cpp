/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_page.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 21:10:59 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/17 21:33:22 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp" // change it by Requst_Header  !!!!
#include "../includes/HttpResponse.hpp"


const std::string GenerateErrorPage(int statuscode, std::string statusmessage)
{
    std::string errorpage;
    errorpage = "<!DOCTYPE html><html><head><title>Error</title><style>body {font-family: Arial, sans-serif;margin: 0;padding: 20px;}h1 {color: #333;font-size: 24px;}p {color: #666;font-size: 16px;}</style></head><body><h1>Error " \
                + std::to_string(statuscode) \
                + "</h1><p>" \
                + statusmessage \
                + "</p></body></html>";
    return errorpage;
}

std::string SearchAboutErrorPage(int statuscode, std::map<int, std::string> err_page) {
    std::string string;
	for (std::map<int, std::string>::iterator it = err_page.begin(); it != err_page.end(); it++)
		if (it->first == statuscode)
			return(it->second);
	return (string);
}

// read err_page file  (my be make it like a globale function to read any file) 
std::string ReadErrorPage(std::string errpage) {
	std::ifstream file(errpage, std::ifstream::binary);
    if (!file.is_open())
    {
    	std::cout << "Error in opening file\n";
    	std::exit(-404);
    }
    file.seekg(0, std::ios::end);
    int Length = file.tellg();
    file.seekg(0, std::ios::beg);
    char buffer[Length + 1];
    file.read(buffer, Length);
	buffer[Length] = '\0';
    file.close();

	return (buffer);
}

/* rest small choise not handling in headers !!!!!!!!!!!!! */
std::string GenerateResponseFromStatusCode(int statuscode) {
    HttpResponse response;
	Request request;
	Server	server;
  
    	// CHECK_STATUS_CODE();
	if (statuscode >= 400){
        response.setVersion("HTTP/1.1");
		response.setStatusCode(statuscode);
		response.setStatusMessage(getReason(statuscode));

		// search about error page in map_error page
		std::string err_page = SearchAboutErrorPage(statuscode, server._map_err_page);
        if (!err_page.empty())
        	response.setBody(ReadErrorPage(err_page));
        else
            response.setBody(GenerateErrorPage(statuscode, getReason(statuscode)));
        
		// new_response.setHeader("Date: ", generateDate());
        response.setHeader("Content-Type", "text/html"); //generateContentType()
        response.setHeader("Content-Length", std::to_string(response.getBodySize()));
		response.setHeader("Conection", request.getConection()); // from map headers (request data)   request.getConection()
	}

	// GENERATE_THE_FINALE_RESPONSE();
    return response.generateResponse(); 
}