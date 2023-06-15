/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 18:33:14 by rarahhal          #+#    #+#             */
/*   Updated: 2023/06/15 21:56:16 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

int main()
{
    Response new_response;

    new_response.setVersion("HTTP/1.1");
    new_response.setStatusCode(200);
    new_response.setStatusMessage("Ok");
    new_response.setHeader("Content-Type", "text/html");
    new_response.setHeader("Content-Length", "1024");
    new_response.setHeader("Connection", "Keep-alaive");
    new_response.setBody("Hello world!\n");

    std::cout << "\n-------Start-------\n\n" << new_response.generateResponse() << "\n\n-------End-------\n" << std::endl;

    return 0;
}