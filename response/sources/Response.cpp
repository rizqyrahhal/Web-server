/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 04:03:03 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/17 06:38:50 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ResponseReturned.hpp"

ResponseReturned::ResponseReturned(server server, request request) {
    _currentIndex = 0;
    Response response;
    *this = response.CreatResponse(server, request);
}

ResponseReturned::ResponseReturned() {
    _currentIndex = 0;
}

/* Function to read a specific size of data from a string or a file */
std::string readData(size_t size, std::string _body, bool _isFile, size_t &currentIndex)
{
    std::string data;
    if (!_isFile)
    {
        size_t remainingSize = _body.size() - currentIndex;
        size_t readSize = std::min(size, remainingSize);
        data = _body.substr(currentIndex, readSize);
        currentIndex += readSize;
    }
    else
    {
        std::ifstream file(_body.c_str());
        if (!file)
            return "";
        file.seekg(currentIndex, std::ios::beg);  /* Move file pointer to the current index */
        char* buffer = new char[size];
        file.read(buffer, size);
        data = std::string(buffer, file.gcount());
        currentIndex = file.tellg();  /* Update current index with the file pointer position */
        delete[] buffer;
        if (data.empty())
            file.close();
    }
    return data;
}

std::string ResponseReturned::GetChanckFromResponse(size_t size_to_read) {
    if (!_headers.empty())
    {
        std::string headToSend = _headers;
        _headers.clear();
        return(headToSend); /* in the first requsted chunck */
    }
    else if (!_body.empty())
    {
        return (readData(size_to_read, _body, _isFile, _currentIndex));
    }
    return (""); /* when return the Empty string the response is finish sending */
}

/* Seters */

void ResponseReturned::setHeaders(std::string headers) {
    _headers = headers;
}

void ResponseReturned::setBody(std::string &body) {
    _body = body;
}

void ResponseReturned::setIsFile(bool &isfile) {
    _isFile = isfile;
}

/* Geters */

std::string ResponseReturned::getHeaders() const {
    return _headers;
}

std::string ResponseReturned::getBody() const {
    return (_body);
}

bool ResponseReturned::getIsFile() const {
    return (_isFile);
}

ResponseReturned::~ResponseReturned() {
    
}
