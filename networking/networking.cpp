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
	if (client.fd_client == -1)
	{
	   std::cerr<<"failed accept method."<<std::endl;
		exit(EXIT_FAILURE);
	}
	if (server::maxfd < client.fd_client)
		server::maxfd = client.fd_client;
	FD_SET(client.fd_client, &server::current);
	return (client);
}

void    run_servers(global & glob)
{
    signal(SIGPIPE, SIG_IGN);
    int resp = 0;
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
				server.client.push_back(accept_new_connection(server));
                // std::cout << "Pushing Clients\n";
            }
            // std::cout << "num clients => " << server.client.size() << std::endl;
			for (size_t i = 0; i < server.client.size() ; i++)
			{
				client &client = server.client[i];
                // std::cout << "CLIENT CHECK => " << client.check << std::endl;
				// IF statement for Request.
				if (FD_ISSET(client.fd_client, &readable) && client.check == 0)
				{
                    resp = client.request_client->read_reqwest(client.fd_client);
                    // std::cout<<client.request_client->max_body_size<<std::endl;
                    // std::cout<<resp<<std::endl;
					// fcntl(client.fd_client, F_SETFL, O_NONBLOCK);
                    client.check = 1;
				}
                // std::cout << "Client -> " << client.fd_client << std::endl;
				else if (FD_ISSET(client.fd_client, &writable) && client.check == 1)
                {
                    if (resp > 0){
                        send(client.fd_client, GenerateResponseFromStatusCode(resp).c_str(), GenerateResponseFromStatusCode(resp).size(), 0);
                    }
                    else if (resp == 0)
                    {
                        Response response;
                        send(client.fd_client, response.CreatResponse(server, *client.request_client).c_str(), response.CreatResponse(server, *client.request_client).size(), 0);
                        //send correct response
                    }
                    close(client.fd_client);
                    FD_CLR(client.fd_client, &server::current);
                    server.client.erase(server.client.begin() + i);

                    //    exit(0);
                }
			}
		}

    }
   
}