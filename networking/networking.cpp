#include "web_serv.hpp" 
#include <sys/socket.h>
void    creat_socket_and_bind(std::map<std::string, std::vector<server> > & map)
{
    std::map<std::string, std::vector<server> >::iterator it;
    for (it = map.begin(); it != map.end(); it++)
    {
        struct addrinfo hints;
        struct addrinfo* res;
        memset(&hints, 0, sizeof(hints));
        int use = 1;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        int status = getaddrinfo(it->second[0].ip_address.c_str(), it->second[0].port[0].c_str(), &hints, &res);
        if (status == -1)
        {
            std::cout<<"getaddrinfo error: "<<std::endl;
            exit(0);
        }
        it->second[0].fd_server = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        FD_SET(it->second[0].fd_server,&server::current);
        FD_SET(it->second[0].fd_server,&server::current2);
        if (server::maxfd < it->second[0].fd_server)
            server::maxfd = it->second[0].fd_server;
        if (it->second[0].fd_server == -1)
        {
            std::cout << "Failed to create socket" << std::endl;
            freeaddrinfo(res);
            continue;
        }

        setsockopt(it->second[0].fd_server ,SOL_SOCKET,SO_REUSEADDR, &use,sizeof(use));
        status = bind(it->second[0].fd_server, res->ai_addr, res->ai_addrlen);
        if (status == -1)
        {
            std::cout << "Failed to bind socket" << std::endl;
            close(it->second[0].fd_server);
            freeaddrinfo(res);
            exit(1);
        }
        freeaddrinfo(res);
   }
}

void     listen_new_connection(std::map<std::string, std::vector<server> > & map)
{
    std::map<std::string, std::vector<server> >::iterator it;
    for (it = map.begin(); it != map.end(); it++)
    {
        if (listen(it->second[0].fd_server, 100) == -1)
        {
            std::cerr << "Failed to listen for connections" << std::endl;
            close(it->second[0].fd_server);
            exit(1);
        }
	    std::cout<<"the server "<< it->second[0].fd_server << " in listen mode "<< "with port "<<it->second[0].port[0] <<" ..."<<std::endl;
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
	FD_SET(client.fd_client, &server::current2);
	return (client);
}

void    run_servers(std::map<std::string, std::vector<server> > & map)
{
    signal(SIGPIPE, SIG_IGN);
    int tmp = 0;
	while (1)
	{
		fd_set writable = server::current;
		fd_set readable = server::current2;
		tmp = select(server::maxfd + 1, &readable, &writable, nullptr, 0);
		if (tmp < 0)
        {
			std::perror("select() Error ");
			continue;
		}
        std::map<std::string, std::vector<server> >::iterator it;
		for (it = map.begin(); it != map.end(); it++)
		{
			if (FD_ISSET(it->second[0].fd_server, &readable))
			{
				it->second[0].client.push_back(accept_new_connection(it->second[0]));
            }

			for (size_t i = 0; i < it->second[0].client.size() ; i++)
			{
                if (it->second[0].client.size() != 0)
                {
				    client &client = it->second[0].client[i];
				    if (FD_ISSET(client.fd_client, &readable) && client.check == 0)
				    {

                        client.resp = client.request_client->read_reqwest(client, it->second);
				        fcntl(client.fd_client, F_SETFL, O_NONBLOCK);
				    }
				    else if (FD_ISSET(client.fd_client, &writable) && client.check == 1)
                    {
                        
                       if (client.resp > 0){
                            if (send(client.fd_client, GenerateResponseFromStatusCode(client.resp, it->second[client.client_in_serv]).c_str(), GenerateResponseFromStatusCode(client.resp, it->second[client.client_in_serv]).size(), 0) <= 0)
                            {
                                std::cout<<"********send status code*********"<<std::endl;
                                client.pr = 1;
                            }
                        }
                        else if (client.resp == 0) {
                            // std::cout << "I WORK ON THIS CLIENT: " << client.fd_client << std::endl;
                            // std::cout << "\n\n************************************************************ SWITCH TO RESPNSE PART ************************************************************\n";
                            if (!client.generateResponseObject) {
                                Response response;
                                client.response_client = response.CreatResponse(it->second[client.client_in_serv], *client.request_client);
                                // std::cout << client.response_client.getHeaders() << client.response_client.readfile() << std::endl;
                                client.generateResponseObject = true;
                            }
        
                            
                            std::string chunck = client.response_client.GetChanckFromResponse(1024);
                            if (chunck.empty()) {
                                client.pr = 1; // chof m3a hada 
                                // sen = true; /// change with client_status_life
                            }
                            else {
                                if (send(client.fd_client, chunck.c_str(), chunck.size(), 0) <= 0)
                                {
                                    // client.response_client.
                                    client.pr = 1;
                                }
                            }
                            
                            // while(!chunck.empty()) {
                            //     chunck = client.response_client.GetChanckFromResponse(255);
                            // }






                            // std::cout << "I SEND RESP TO THIS USER: " << client.fd_client << std::endl;

                            // std::cout << "\n###################################################################################################################################################\n\n";
                            // send correct response
                            // client.pr = send_video(client);
                        }
                        if (client.resp == -1 || client.resp > 0 || client.pr) 
                        {
                            std::cout<<"client "<<client.fd_client<<" dropped succesfolly and MAXFD == "<<server::maxfd<<std::endl;
                            close(client.fd_client);
                            FD_CLR(client.fd_client, &server::current);
                            FD_CLR(client.fd_client, &server::current2);
                            delete client.request_client;
                            it->second[0].client.erase(it->second[0].client.begin() + i);
                            std::cout<<"number of client : "<<it->second[0].client.size()<<std::endl;

                        }  
                    }
                }
			}
		}
    }
   
}
