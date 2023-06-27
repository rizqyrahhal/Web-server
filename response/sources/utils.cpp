/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 00:10:22 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/27 08:48:48 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

// <!-- DOCTYPE html -->
// <html>
// <head><title>Directory Listing</title></head>
// <body>
// <h1>Directory Listing</h1>
// <ul>
// <li><a href="./www/app/html/.">.</a></li>
// <li><a href="./www/app/html/..">..</a></li>
// <li><a href="./www/app/html/images">images</a></li>
// <li><a href="./www/app/html/logo.png">logo.png</a></li>
// <li><a href="./www/app/html/Music.mp4">Music.mp4</a></li>
// <li><a href="./www/app/html/scripts">scripts</a></li>
// <li><a href="./www/app/html/styles">styles</a></li>
// <li><a href="./www/app/html/videoplayback.mp4">videoplayback.mp4</a></li>
// </ul>
// </body>
// </html>


std::string generateAutoindexFile(std::string requestedSource) {
    std::cout << "in GenerateAoutoindex: " << requestedSource << std::endl;
    /* this code begin with it to search about lest of directory files */
    std::string indexhtml = "<!-- DOCTYPE html --><html><head><title>Directory Listing</title></head><body><h1>Directory Listing</h1><ul>";
    DIR* dr = opendir(requestedSource.c_str());
    if (dr != NULL)
    {
        struct dirent *dir = NULL;
        while ((dir = readdir(dr)))
        {
            indexhtml += "<li><a href=\"" + std::string(dir->d_name) + "\">" + dir->d_name + "</a></li>";
            // if (dir->d_type != DT_REG)
                // std::cout << "name: " << dir->d_name << std::endl;
        //     if (dir == "index.html")
        //     {
        //         *path += "index.html";
        //         return true;
        //     }
        }
        indexhtml += "</ul></body></html>";
        closedir(dr);
    }
    // std::cout << ""
    return  (indexhtml);
}

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