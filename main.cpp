#include "web_serv.hpp" 

void    creat_socket_and_bind(global & glob)
{
    for (size_t i = 0; i < glob.server.size(); i++)
    {
        server &serv = glob.server[i];

        serv.fd_server = socket(AF_INET, SOCK_STREAM, 0);
        if (serv.fd_server == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        exit(1);
        }

        bzero(&serv.address, sizeof(serv.address));
        serv.address.sin_family = AF_INET;
        serv.address.sin_port = htons(serv.port);
        if (inet_pton(AF_INET, serv.ip_address.c_str(), &(serv.address.sin_addr)) <= 0) {
        perror("Invalid IP address");
        exit(EXIT_FAILURE);
        }

        FD_SET(serv.fd_server, &server::current);
        server::maxfd = std::max(serv.fd_server, server::maxfd);
        if (bind(serv.fd_server, (struct sockaddr*)&serv.address, sizeof(serv.address)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(serv.fd_server);
        exit(1);
        }

    }
    
}

void     listen_new_connection(global & glob)
{
    for (size_t i = 0; i < glob.server.size(); i++)
    {
        if (listen(glob.server[i].fd_server, 100) == -1) {
            std::cerr << "Failed to listen for connections" << std::endl;
            close(glob.server[i].fd_server);
            exit(1);
        }
	std::cout<<"the server "<< glob.server[i].fd_server << " in listen mode "<< "with port "<< glob.server[i].port<<std::endl;
    }

}

client accept_new_connection(server& serv)
{
    client client;
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
		if (!tmp)
			continue;
		for (int  j = 0; j < glob.server.size(); j++)
		{
			server &server = glob.server[j];
			if (FD_ISSET(server.fd_server, &readable))
			{
				server.client.push_back(accept_new_connection(server));
			}
			for (int i = 0; i < server.client.size() ; i++)
			{
				client &client = server.client[i];
				// IF statement for Request.
				if (FD_ISSET(client.fd_client, &readable))
				{
					fcntl(client.fd_client, F_SETFL, O_NONBLOCK);
					
				}
				// IF statement for Response.
				else if(i >= 0  && FD_ISSET(client.fd_client, &writable))
				{
                    
				}
			}
		}
	}
}

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
        creat_socket_and_bind(global);
        listen_new_connection(global);
        run_servers(global);
    }
    catch(const std::exception& e)
    {
        std::cout<<"ERROR: "<<e.what()<<std::endl;
    }
}
