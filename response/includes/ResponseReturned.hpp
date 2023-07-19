/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseReturned.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 07:01:11 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/19 00:58:12 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "responsePart.hpp"

class request;
class server;
class locations;

const std::string generatBody(std::string _requestedSource);

class ResponseReturned
{
    private:
        std::string _headers;
        std::string _body; // here the body in the case of body in string like cgi or errors or autoindex ..  || the file name if the body in file
        bool _isFile;
        size_t _currentIndex;
    public:
        // ResponseReturned(server server, request request);
        ResponseReturned();
        std::string GetChanckFromResponse(size_t size_to_read);
        // static std::string readData(std::string& str, int size);

        /* Seters */
        void setHeaders(std::string headers);
        void setBody(std::string &body);
        void setIsFile(bool &isfile);
        /* Geters */
        std::string getHeaders() const;
        std::string getBody() const;
        bool getIsFile() const;
        std::string readfile() { // temporare function
            if (_isFile)
                return(generatBody(_body));
            return (_body);
        }
        ~ResponseReturned();
};