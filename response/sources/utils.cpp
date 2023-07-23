/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 00:10:22 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/23 19:50:24 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"

bool checkIndexInsidDerctory(std::string *path) {
    std::string indexhtml = *path + "index.html";
    std::string indexphp = *path + "index.php";
    std::string indexpy = *path + "index.py";
    if (access(indexhtml.c_str(), 0) == 0) {
        *path = indexhtml;
        return true;
    }
    if (access(indexphp.c_str(), 0) == 0) {
        *path = indexphp;
        return true;
    }
    if (access(indexpy.c_str(), 0) == 0) {
        *path = indexpy;
        return true;
    }
    return false;
}

/* function to response body */
const std::string generatBody(std::string _requestedSource) {
    std::ifstream file(_requestedSource.c_str(), std::ifstream::binary);
    if (!file.is_open())
        throw(403);

    file.seekg(0, std::ios::end);
    int length = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(length);
    file.read(&buffer[0], length);
    file.close();

    return (std::string(buffer.begin(), buffer.end()));
}

int calculeBodySize(std::string _requestedSource) {
    std::ifstream file(_requestedSource.c_str(), std::ifstream::binary);
    if (!file.is_open())
        throw(403);

    file.seekg(0, std::ios::end);
    int length = file.tellg();
    file.seekg(0, std::ios::beg);

    file.close();
    return (length);
}

std::string getCurrentDate() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);

    std::ostringstream oss;
    oss << std::put_time(localTime, "%a, %d %b %Y %H:%M:%S %Z");

    return oss.str();
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos)
        return "";

    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::pair<std::string, std::string> parseHeader( const std::string& line )  {
	std::string _key, _value;
	size_t _end = 0;
	std::string _line = trim(line);

	while (_end < _line.length() && _line[_end] != ':')	++_end;
	_key =  trim(_line.substr(0, _end));

	if (_end < _line.length())
		_value = trim(_line.substr(++_end));

	return std::make_pair(_key, _value);
}

std::string getRandomString(int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int charsetSize = sizeof(charset) - 1;

    std::string randomString;
    randomString.reserve(length);

    std::srand(static_cast<unsigned int>(std::time(0)));

    for (int i = 0; i < length; ++i) {
        randomString += charset[std::rand() % charsetSize];
    }

    return randomString;
}