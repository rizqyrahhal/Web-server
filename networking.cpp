#include "web_serv.hpp"


#include <sys/socket.h>
void    creat_socket_and_bind(global & glob)
{
    for (size_t i = 0; i < glob.server.size(); i++)
    {
        server &serv = glob.server[i];
        struct sockaddr_in addr;
        struct addrinfo hints;
        struct addrinfo* res;
        memset(&hints, 0, sizeof(hints));
        int use = 1;
        serv.fd_server = socket(AF_INET, SOCK_STREAM, 0);
        FD_SET(serv.fd_server,&server::current);
        if (server::maxfd < serv.fd_server)
            server::maxfd = serv.fd_server;
        if (serv.fd_server == -1)
        {
            std::cout << "Failed to create socket" << std::endl;
            continue;
        }

        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        int status = 0;
        if ((status = getaddrinfo(serv.ip_address.c_str(), "http", &hints, &res)) != 0)
        {
            std::cout<<"getaddrinfo error: "<<std::endl;
            exit(0);
        }

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(serv.port);

        setsockopt(serv.fd_server ,SOL_SOCKET,SO_REUSEADDR, &use,sizeof(use));
        std::cout << "serv.fd_server " << serv.fd_server  << std::endl;
        if (bind(serv.fd_server, reinterpret_cast<const sockaddr *>(&addr) , sizeof(addr)) == -1)
        {
            std::cout << "Failed to bind socket" << std::endl;
            close(serv.fd_server);
            exit(1);
        }
        freeaddrinfo(res);
        std::cout<<"---------finish----------"<<std::endl;
   }
}

void     listen_new_connection(global & glob)
{
    for (size_t i = 0; i < glob.server.size(); i++)
    {
        if (listen(glob.server[i].fd_server, 100) == -1)
        {
            std::cerr << "Failed to listen for connections" << std::endl;
            close(glob.server[i].fd_server);
            exit(1);
        }
	    std::cout<<"the server "<< glob.server[i].fd_server << " in listen mode "<< "with port "<< glob.server[i].port <<" ..."<<std::endl;
    }
}

client accept_new_connection(server& serv)
{
    client client(serv.client_body_size);
	client.fd_client = accept(serv.fd_server, (struct sockaddr *)&client.client_address, &client.clientaddrlenght);
    fcntl(client.fd_client, F_SETFL, O_NONBLOCK);
	if (client.fd_client == -1)
	{
	   std::cerr<<"failed accept method."<<std::endl;
	// exit(EXIT_FAILURE);
	}
	if (server::maxfd < client.fd_client)
		server::maxfd = client.fd_client;
	FD_SET(client.fd_client, &server::current);
	return (client);
}

int    send_video(client & client)
{
    int fd = 0;
    // std::cout <<" i want to send video --->>>"<<std::endl;
    char *readvideo = new char[1024];
    if (client.p == 0)
    {
        fd = open("/Users/araysse/Desktop/video.mp4", 0);
        client.readFd = fd;
        std::string header = "HTTP/1.1 200 OK\r\n"
                       "Content-Type: video/mp4\r\n"
                       "Content-Length: 32457473\r\n"
                       "Connection: closed\r\n\r\n";
        send(client.fd_client, header.c_str(), header.size(), 0);
        client.p++;
        return (0);
    }
    if (client.readFd <= 0)
        return (-1);
    int rd = read(client.readFd, readvideo, 1024);
    if (rd <= 0)
       return(-1);
        // Send the image data
    int snd = send(client.fd_client, readvideo, rd, 0);
    if (snd <= 0)
    {
        close(fd);
        return (1);
    }
    return (0);
}

void    run_servers(global & glob)
{
    signal(SIGPIPE, SIG_IGN);
    int tmp = 0;
	while (1)
	{
		fd_set writable = server::current;
		fd_set readable = server::current;
		tmp = select(server::maxfd + 1, &readable, &writable, nullptr, 0);
		if (tmp < 0) {
			std::perror("select() Error ");
			continue;
		}
		for (size_t  j = 0; j < glob.server.size(); j++)
		{
			server &server = glob.server[j];
			if (FD_ISSET(server.fd_server, &readable))
			{
                std::cout << "aaa Clients " << server.client.size() << "\n";
                client client = accept_new_connection(server);
                std::cout << "bbb Clients " << server.client.size() << "\n";
				server.client.push_back(client);
                std::cout << "Pushing Clients fd_client : "<<client.fd_client<<std::endl;
                // exit(0);
            }
            // std::cout << "num clients => " << server.client.size() << std::endl;
			for (size_t i = 0; i < server.client.size() ; i++)
			{   
                if (server.client.size() != 0)
                {
				    client &client = server.client[i];
                        //  std::cout << "CLIENT CHECK => " << client.check << std::endl;
                    // std::cout << "clients => " << server.client.size()  << " i = " << i << std::endl;
				    // IF statement for Request.
				    if (FD_ISSET(client.fd_client, &readable) && client.check == 0)
				    {
                        client.resp = client.request_client->read_reqwest(client);
                        // std::cout<<client.request_client->max_body_size<<std::endl;
                        // std::cout<<client.resp<<std::endl;
				    	// fcntl(client.fd_client, F_SETFL, O_NONBLOCK);
                        client.check = 1;
				    }
                    // std::cout << "Client -> " << client.fd_client << std::endl;
				    else if (FD_ISSET(client.fd_client, &writable) && client.check == 1)
                    {
                        int pr = 0;
                        if (client.resp > 0){
                            send(client.fd_client, GenerateResponseFromStatusCode(client.resp).c_str(), GenerateResponseFromStatusCode(client.resp).size(), 0);
                        }
                        else if (client.resp == 0){
                            // std::cout<<"send video ----->>>>>"<<std::endl;
                            pr = send_video(client);
                            // std::cout<<"sift chi haja hna ::::"<<std::endl;
                            //send correct response
                            //  std::cout<<"client "<<client.fd_client<<" in sending mode"<<std::endl;
                        }
                        // std::cout << "here "  << pr << "\n";
                        if (client.resp > 0 || pr) {
                            std::cout<<"client "<<client.fd_client<<" dropped succesfolly"<<std::endl;
                            close(client.fd_client);
                            FD_CLR(client.fd_client, &server::current);
                            server.client.erase(server.client.begin() + i);

                        }                   
                    }
                }
			}
		}

    }
   
}