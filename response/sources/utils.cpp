/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rizqy <rizqy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 00:10:22 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/22 16:11:37 by rizqy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/responsePart.hpp"


bool checkIndexInsidDerctory(std::string *path) {
    std::string indexhtml = *path + "index.html"; // this is hard code but work less like that
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

std::string getCurrentDate() { // this function is Just harde code it have an forbiden functionalite
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
	// key
	while (_end < _line.length() && _line[_end] != ':')	++_end;
	_key =  trim(_line.substr(0, _end));

	// value
	if (_end < _line.length())
		_value = trim(_line.substr(++_end));

    // std::cout << "_KEY: " << _key << " _VALUE: " << _value << std::endl;

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





                    // else if (resp == 0)
                    // {
                    //     std::cout << "\n\n************************************************************ SWITCH TO RESPNSE PART ************************************************************\n";
                    //     Response response;
                    //     ResponseReturned res = response.CreatResponse(server, *client.request_client);
                    //     // std::cout << "isFile: " << res.getIsFile() << std::endl << "BodyFile: " << res.getBody() << std::endl;
                    //     //     std::cout << "\n***** Response ***** \n" << (res.getHeaders() + res.readfile()) << "\n----------------------------------\n";
                    //     // int sending = send(client.fd_client, (res.getHeaders()).c_str(), (res.getHeaders()).size(), 0);
                    //     std::string chunck = res.GetChanckFromResponse(255);
                    //     while(!chunck.empty()) {
                    //         int sending = send(client.fd_client, chunck.c_str(), chunck.size(), 0);
                    //         (void)sending;
                    //         // std::cout << "I SEND RESP TO THIS USER: " << client.fd_client << "\nSENDING: " <<  sending << std::endl;
                    //         chunck = res.GetChanckFromResponse(255);
                    //     }
                    //     std::cout << "\n###################################################################################################################################################\n\n";
                    //     //send correct response
                    //     sen = true; /// change with client_status_life
                    // }