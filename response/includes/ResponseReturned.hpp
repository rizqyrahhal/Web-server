/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseReturned.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 07:01:11 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/20 02:57:30 by rarahhal         ###   ########.fr       */
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
        std::string _body; /* The body have two cases, the path to file, or the content when have small size */
        bool _isFile;
        size_t _currentIndex;
    public:
        ResponseReturned();
        std::string GetChanckFromResponse(size_t size_to_read);
        /* Seters */
        void setHeaders(std::string headers);
        void setBody(std::string &body);
        void setIsFile(bool &isfile);
        /* Geters */
        std::string getHeaders() const;
        std::string getBody() const;
        bool getIsFile() const;
        std::string readfile() { /* this function Just to print the body for debug */
            if (_isFile)
                return(generatBody(_body));
            return (_body);
        }
        ~ResponseReturned();
};