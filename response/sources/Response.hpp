/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 05:29:02 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/11 06:31:57 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>

class Response
{
    protected:
        std::string startLine;
        std::string headers;
        bool status; //status of resonse finish or need to upload some thing (open read send or creat appended)
        bool isCgi;// this is true just when the cgi response generated is succeffule
        int fdCreatedByCgi;
        std::string fileName; // here the file name of the body or the name of the file be created to upload Post body
    public:
        Response();
        void creatStartLine(size_t status_code);
        std::string concatentStartLineWithHeaders();
        std::string readBodyFromFileBySpeceficeSize(size_t size_to_read);
        /* Geters */
        std::string getStartLine() const;
        std::string getHeaders() const;
        bool getStatus() const;
        bool getIsGgi() const;
        // int getFdCreatedByCgi() const;
        // std::string getFileName() const;
        ~Response();
};


/* this class implement to be like a pipe line between the network part and the response generated part 
for the reason of switch data by the minimum resource and efficition time 
hte place to be set this is in the request class created when need to creat any response in the network part 
inhiret from it in the responsePart class fill all the needed and possebele variable and data insid it and take its to destroyed when finishe the network side*/