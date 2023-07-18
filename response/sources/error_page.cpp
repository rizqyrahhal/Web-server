/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_page.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 21:10:59 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/14 02:48:15 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp" // change it by Requst_Header  !!!!
#include "../includes/HttpResponse.hpp"


const std::string GenerateErrorPage(int statuscode, std::string statusmessage) {
    std::string errorpage;
    errorpage = "<!DOCTYPE html><html><head><title>Error</title><style>body {font-family: Arial, sans-serif;margin: 0;padding: 20px;}h1 {color: #333;font-size: 24px;}p {color: #666;font-size: 16px;}</style></head><body><h1>Error " \
                + std::to_string(statuscode) \
                + "</h1><p>" \
                + statusmessage \
                + "</p></body></html>";
    return errorpage;
}

/* this  function for error after determine matchedlocation (using inside ResponseGeneratedFromStatusCode() function) */
std::string SearchAboutErrorPageFormTowPlaces(int statuscode, std::map<int, std::string> g_err_page, std::map<int, std::string> l_err_page) {
    std::string string;
	for (std::map<int, std::string>::iterator l_it = l_err_page.begin(); l_it != l_err_page.end(); l_it++)
		if (l_it->first == statuscode)
			return(l_it->second);
	for (std::map<int, std::string>::iterator g_it = g_err_page.begin(); g_it != g_err_page.end(); g_it++)
		if (g_it->first == statuscode)
			return(g_it->second);
	return (string);
}

// thhis function when change in GenerateResponseFromStatusCode() by SearchAboutErrorPageFormTowPlaces() delete it
std::string SearchAboutErrorPage(int statuscode, std::map<int, std::string> g_err_page) {
    std::string string;
	for (std::map<int, std::string>::iterator g_it = g_err_page.begin(); g_it != g_err_page.end(); g_it++)
		if (g_it->first == statuscode)
			return(g_it->second);
	return (string);
}

/* read err_page file */
std::string ReadErrorPage(std::string errpage) {
	std::ifstream file(errpage.c_str(), std::ifstream::binary);
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
//  the header in htis function is fixed for all time
// this function need to add server to get from it the error page map
std::string GenerateResponseFromStatusCode(int statuscode) { // adding argement to take server for the error page
    HttpResponse response;
	// Request request;
	Server	server;
  
    	// CHECK_STATUS_CODE();
	if (statuscode >= 400){
        
        /* Start line element */
        response.setVersion("HTTP/1.1");
		response.setStatusCode(statuscode);
		response.setStatusMessage(getReason(statuscode));

        /* set default headers */
		// new_response.setHeader("Date: ", generateDate());
		response.setHeader("Conection", "close"); // from map headers (request data)   request.getConection()

		// search about error page in map_error page
		std::string err_page = SearchAboutErrorPage(statuscode, server._map_err_page); // this function need to be SearchAboutErrorPageFormTowPlaces()
        if (!err_page.empty())
        	response.setBody(ReadErrorPage(err_page));
        else
            response.setBody(GenerateErrorPage(statuscode, getReason(statuscode)));
        
        response.setHeader("Content-Type", "text/html"); //generateContentType()
        response.setHeader("Content-Length", std::to_string(response.getBodySize()));
	}

	// GENERATE_THE_FINALE_RESPONSE();
    return response.generateResponse();
}
