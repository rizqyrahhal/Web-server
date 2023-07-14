/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 05:29:02 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/14 01:11:58 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

const std::string generatBody(std::string _requestedSource);

class ResponseReturned
{
    private:
        std::string _headers;
        std::string _body; // here the body in the case of body in string like cgi or errors or autoindex ..  || the file name if the body in file
        bool _isFile;
        size_t _currentIndex;
    public:
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