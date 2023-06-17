/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 18:33:14 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/17 17:11:42 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responsePart.hpp"

void network(void);


int main()
{
	Server server;
	Request request;

	// std::cout 

    network();
    // Response new_response;

    // new_response.setVersion("HTTP/1.1");
    // new_response.setStatusCode(200);
    // new_response.setStatusMessage("Ok");
    // new_response.setHeader("Content-Type", "text/html");
    // new_response.setHeader("Content-Length", "1024");
    // new_response.setHeader("Connection", "Keep-alaive");
    // new_response.setBody("Hello world!\n");

    // std::cout << "\n-------Start-------\n\n" << new_response.generateResponse() << "\n\n-------End-------\n" << std::endl;

    return 0;

}































// //  the Request class
// class Request
// {
//     private:
// 		std::string _method;
// 		std::string _uri;
// 		std::string _version;
//         std::string _conection;
// 		std::map<std::string, std::string> _headers;
//     public:
// 		Request(){
//             _method = "GET";
//             _uri = "/index.html";
//             _version = "HTTP/1.1";
//             _conection = "close";
// 			_headers[""] = "";
//         }
//         std::string getMethod() const {
//             return _method;
//         }
//         std::string getUri() const {
//             return _uri;
//         }
//         std::string getVersion() const
//         {
//             return _version;
//         }
//         std::string getConection() const {
//             return _conection;
//         }
// 		~Request(){}
// };

// // the Location class
// class Locations
// {
// 	public:
// 		std::string _name;
// 		std::string _root;
// 		std::vector<std::string> _allow_methods;
// 		std::string _autoindex;
// 		std::map<std::string, std::string> _cgi;
// 		std::map<int, std::string> _redirect;
// 	public:
// 		Locations() {
// 			_name = "localhost";
// 			_root = "www/";
// 			_allow_methods.push_back("GET");
// 			_allow_methods.push_back("POST");
// 			_allow_methods.push_back("DELETE");
// 			_autoindex = "on";
// 			// _cgi ;
// 			// _redirect ;
// 		}
// 		~Locations(){}
// };

// // the Server class
// class Server
// {
//     public:
// 	    int _port;
// 	    std::string _ip_address;
// 	    std::string _server_name;
// 	    std::vector<Locations> _locations;
// 	    std::map<int, std::string> _map_err_page;
// 	    int _client_body_size;
// 	public:
// 	    Server(){
//             _port = 0000;
//             _ip_address = "127.0.0.1";
// 			_server_name = "localhost";
// 			// _locations ;
// 			// _map_err_page ;
// 			// _client_body_size ;
//         }
// 	    ~Server(){}
// };