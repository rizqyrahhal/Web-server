#include "web_serv.hpp" 
#include <sys/socket.h>
void    creat_socket_and_bind(global & glob)
{
    for (size_t i = 0; i < glob.server.size(); i++)
    {
        server &serv = glob.server[i];
        // struct sockaddr_in addr;
        struct addrinfo hints;
        struct addrinfo* res;
        memset(&hints, 0, sizeof(hints));
        std::cout<<"hi *_*_*_*"<<std::endl;
        int use = 1;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        int status = getaddrinfo(serv.ip_address.c_str(), serv.port.c_str(), &hints, &res);
        if (status == -1)
        {
            std::cout<<"getaddrinfo error: "<<std::endl;
            exit(0);
        }
        serv.fd_server = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        FD_SET(serv.fd_server,&server::current);
        if (server::maxfd < serv.fd_server)
            server::maxfd = serv.fd_server;
        if (serv.fd_server == -1)
        {
            std::cout << "Failed to create socket" << std::endl;
            freeaddrinfo(res);
            continue;
        }

        setsockopt(serv.fd_server ,SOL_SOCKET,SO_REUSEADDR, &use,sizeof(use));
        std::cout << "serv.fd_server " << serv.fd_server  << std::endl;
        status = bind(serv.fd_server, res->ai_addr, res->ai_addrlen);
        if (status == -1)
        {
            std::cout << "Failed to bind socket" << std::endl;
            close(serv.fd_server);
            freeaddrinfo(res);
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
		// exit(EXIT_FAILURE);
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
    bool sen = false;
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
                std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  Pushing Clients @@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
				server.client.push_back(accept_new_connection(server));
                std::cout << "########### number of client:  " << server.client.size() << std::endl;
            }
            // std::cout << "########### FD_client:  " << server.client.fd_client << std::endl;
            // std::cout << "num clients => " << server.client.size() << std::endl;
			for (size_t i = 0; i < server.client.size() ; i++) // in this i <= Just to evet loop
			{
                // std::cout << "++++++++++++++++++++++++++++++++\n";
                if (server.client.size() != 0)
                {
				    client &client = server.client[i];
                    // std::cout << "CLIENT CHECK => " << client.check << std::endl;
				    // IF statement for Request.
				    if (FD_ISSET(client.fd_client, &readable) && client.check == 0 )
				    {
                        resp = client.request_client->read_reqwest(client);
                        // std::cout<<client.request_client->max_body_size<<std::endl;
                        // std::cout<<resp<<std::endl;
				    	// fcntl(client.fd_client, F_SETFL, O_NONBLOCK);
                        // client.check = 1;
				    }
                    // std::cout << "Client -> " << client.fd_client << std::endl;
				    else if (FD_ISSET(client.fd_client, &writable) && client.check == 1)
                    {
                        if (resp > 0){
                            std::cout<<"hello from statc code : "<<std::endl;
                            send(client.fd_client, GenerateResponseFromStatusCode(resp).c_str(), GenerateResponseFromStatusCode(resp).size(), 0);
                        }
                        else if (resp == 0)
                        {
                            std::cout << "\n\n************************************************************ SWITCH TO RESPNSE PART ************************************************************\n";
                            Response response;
                            std::string res = response.CreatResponse(server, *client.request_client);
                                std::cout << "\n***** Response ***** \n" << res << "\n----------------------------------\n";
                            int sending = send(client.fd_client, res.c_str(), res.size(), 0);
                            std::cout << "I SEND RESP TO THIS USER: " << client.fd_client << "\nSENDING: " <<  sending << std::endl;
                            std::cout << "\n###################################################################################################################################################\n\n";
                            //send correct response
                            sen = true; /// change with client_status_life
                        }
                        if (resp > 0 || sen) {
                            close(client.fd_client);
                            FD_CLR(client.fd_client, &server::current);
                            server.client.erase(server.client.begin() + i);

                            std::cout << "server.client.size() : " << server.client.size() << "   ) :)\n";
                            // exit (0);
                        }
                        //    exit(0);
                    }
                    // else{
                    //     close(client.fd_client);
                    //     FD_CLR(client.fd_client, &server::current);
                    //     server.client.erase(server.client.begin() + i);
                    //     std::cout << "server.client.size() : " << server.client.size() << "   ) :)\n";
                    // }
                }
                else
                    break;
			}
		}

    }
   
}

