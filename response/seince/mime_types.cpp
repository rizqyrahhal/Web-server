/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mime_types.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 17:59:54 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/15 18:05:26 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    } else {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        std::exit(-1);
    }

    return mimeTypes;
}

int main()
{
	std::map<std::string, std::string> map = readMimeTypes("./mime.types");

	for(std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); it++) {
		std::cout << it->first << "=" << it->second << std::endl;
	}
	return 0;
}