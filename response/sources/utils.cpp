/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 00:10:22 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/03 12:29:10 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"


bool checkIndexInsidDerctory(std::string *path) {
    std::string index = *path + "index.html";
    if (access(index.c_str(), 0) == 0) {
        *path = index;
        return true; 
    }
    return false;
}

/* function to response body */
const std::string generatBody(std::string _requestedSource) {
    std::ifstream file(_requestedSource.c_str(), std::ifstream::binary);
    if (!file.is_open())
    {
    	std::cout << "Error in opening file\n";    // this error and any other like its need to handel in parssing and exit if an problem (ofxorce check with it in my code to evet any carash)
    	std::exit(-404);
    }

    file.seekg(0, std::ios::end);
    int length = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(length);
    file.read(&buffer[0], length);
    file.close();

    return (std::string(buffer.begin(), buffer.end()));
}