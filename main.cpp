#include "web_serv.hpp" 

// int main() {
//     // Create a socket
//     int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (serverSocket == -1) {
//         std::cerr << "Failed to create socket" << std::endl;
//         return 1;
//     }

//     // Bind the socket to a specific address and port
//     sockaddr_in serverAddress;
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_port = htons(8080); // Port number
//     serverAddress.sin_addr.s_addr = INADDR_ANY; // Bind to any available network interface

//     if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
//         std::cerr << "Failed to bind socket" << std::endl;
//         close(serverSocket);
//         return 1;
//     }

//     // Listen for incoming connections
//     if (listen(serverSocket, 5) == -1) {
//         std::cerr << "Failed to listen for connections" << std::endl;
//         close(serverSocket);
//         return 1;
//     }

//     std::cout << "Server listening on port 8080..." << std::endl;
//     char buffer[1024];
//     char *header = "HTTP/1.1 200 OK\r\n"
//                     "Content-Type: video/mp4\r\n"
//                     "Content-Length: 9973881\r\n"
//                     "Connection: closed\r\n\r\n";

//     // Accept incoming connections
//     std::vector<int> cliaccept;
//     int max_socketfd = 1;
//     fd_set current;
//     fd_set readclientstruct;
//     fd_set writeclientstruct;
//     FD_ZERO(&current);
//     FD_SET(serverSocket, &current);
//     while (1) {
//         int i = 0;
//         while(i < cliaccept.size()){
//             FD_SET(cliaccept[i], current);
//             i++;
//         }
//         FD_ZERO(&readclientstruct);
//         FD_ZERO(&writeclientstruct);
//         readclientstruct = current;
//         writeclientstruct = current;
//         select(max_socketfd + 1, &readclientstruct, &writeclientstruct ,null);
//         // while(i < servervec.size())
//         // {
//             if (FD_ISSET(serverSocket, &readclientstruct))
//             {
//                 //accept new connection
//                 int client = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength)
//             }
//             while(client)
//             {
//                 if (FD_ISSET(CLINNET_ID,  &readclientstruct))
//                 {
//                     //UNCOMMING REQUESST
//                 }
//                 else if (FD_ISSET(CLINNET_ID, &writeclientstruct))
//                 {
//                     //You have to send Response
                    
//                 }
//             }
 
//         // }
//     }

//     close(serverSocket);

//     return 0;
// }

        // sockaddr_in clientAddress;
        // socklen_t clientAddressLength = sizeof(clientAddress);

        // int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);

        // if (clientSocket == -1) {
        //     std::cerr << "Failed to accept connection" << std::endl;
        //     close(serverSocket);
        //     return 1;
        // }
        // std::cout << "Client connected: " << clientSocket << std::endl;

        // std::cout << "Header: " << header << " Header length: " << strlen(header) << std::endl;
        // if (send(clientSocket, header, strlen(header), 0) <= 0) {
        //     std::cout << "Failed to send header" << std::endl;
        //     close(clientSocket);
        //    return 1;
        // }

        // int fd = open("./Music.mp4", O_RDONLY);
        // struct stat info;
        // fstat(fd, &info);
        // char *bufff = new char[1024];
        // while (read(fd, bufff, 1024) != 0)
        // {
        // // int rd = read(fd, bufff, info.st_size);
        // // Send the image data
        // send(clientSocket, bufff, 1024, 0);
        // }


        // close(clientSocket);


int main(int ac, char **av)
{
 
    if (ac != 2)
    {
        std::cout<<"invalid argument !"<<std::endl;
        return(0);
    }
    try
    {
        global global;
        ft_parce_config(av, global);
        std::cout<<"hello l3awny\n";
    }
    catch(const std::exception& e)
    {
        std::cout<<"ERROR: "<<e.what()<<std::endl;
    }
}