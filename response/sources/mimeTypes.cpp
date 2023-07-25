/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mimeTypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 13:49:31 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/25 01:47:14 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <iterator>

std::map<std::string, std::string> readMimeTypes(const std::string& filePath) {
    std::map<std::string, std::string> mimeTypes;

    std::ifstream file(filePath);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if ((line.empty() || line[0] == '#') || line == "types {")
                continue;

            std::istringstream iss(line);
            std::string mimeType;
            std::string extensions;

            if (iss >> mimeType) {
                while (iss >> extensions) {
                    extensions.erase(std::remove_if(extensions.begin(), extensions.end(), ::isspace), extensions.end());
                    if (!extensions.empty()) {
                        std::istringstream extStream(extensions);
                        std::string extension;
                        while (std::getline(extStream, extension, ' ')) {
							extension.insert(0, ".");
							if (extension[extension.size() - 1] == ';')
								extension.erase(extension.size() - 1, extension.size());
                            mimeTypes[extension] = mimeType;
                        }
                    }
                }
            }
        }
        file.close();
    }
    else {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }

    return mimeTypes;
}



std::string getFileExtantion(std::string requestedsource) {
    size_t position = requestedsource.find_last_of(".");
    if (position != std::string::npos)
        return (requestedsource.substr(position, requestedsource.size()));
    else
        return "";
}

std::string getMimeType(std::map<std::string, std::string> mimetypes, std::string fileextantion) {
    std::map<std::string, std::string>::iterator it = mimetypes.begin();
    for (; it != mimetypes.end(); it++) {
        if (it->first == fileextantion)
            return (it->second);
    }
    return ("");
}

std::string getExtantion(std::map<std::string, std::string> mimetypes, std::string content_type) {
    std::map<std::string, std::string>::iterator it = mimetypes.begin();
    for (; it != mimetypes.end(); it++) {
        if (it->second == content_type)
            return (it->first);
    }
    return ("");
}

/* this function fill the cgi types */
void fillMimeTypes(std::map<std::string, std::string> &mimeTypes) {
    mimeTypes[".php"] = "application/x-httpd-php";
    mimeTypes[".py"] = "application/x-python-code";
}
