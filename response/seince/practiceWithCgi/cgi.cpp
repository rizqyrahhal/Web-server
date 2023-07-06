/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarahhal <rarahhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 21:48:56 by rarahhal          #+#    #+#             */
/*   Updated: 2023/07/07 00:14:56 by rarahhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <unistd.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
        char* argv[] = {
            (char*)"php-cgi",
            (char*)"-f",
            (char*)"./test.php",
            nullptr
        };
    pid_t pid = fork();

    if (pid == 0)
    {
        execve("./php-8.1.21/sapi/cgi/php-cgi.1.in", argv, NULL);
        perror("execve");
        exit(-1);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
    return 0;
}