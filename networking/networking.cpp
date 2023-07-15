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

int    send_video(client & client)
{

    // std::cout <<" i want to send video --->>>"<<std::endl;
    char *readvideo = new char[1024];
    if (client.p == 0)
    {

        client.readFd = open("/Users/araysse/Desktop/arsenal.png", 0);
        std::string header = "HTTP/1.1 200 OK\r\n"
                       "Content-Type: image/png\r\n"
                       "Content-Length: 213946\r\n"
                       "Connection: closed\r\n\r\n";
        if (send(client.fd_client, header.c_str(), header.size(), 0) <= 0)
        {
            // std::cout<<"send salat******"<< "returned value : "<<snd<<std::endl;
            std::cout<<strerror(errno)<<"\n\n\n"<<std::endl;
            close(client.readFd);
            return (1);
        }
        client.p++;
        return (0);
    }
    if (client.readFd <= 0)
    {
        std::cout<<"open video failed"<<std::endl;
        return (1);
    }
    int rd = read(client.readFd, readvideo, 1024);
    if (rd <= 0)
    {
       std::cout<<"read salat"<<std::endl;
       close(client.readFd);
       return(1);
    }
    int snd = send(client.fd_client, readvideo, rd, 0);
    // std::cout<<"send == "<<snd<<std::endl;
    if (snd <= 0)
    {
        std::cout<<"send salat******"<< "returned value : "<<snd<<std::endl;
        std::cout<<strerror(errno)<<std::endl;
        close(client.readFd);
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
		if (tmp < 0)
        {
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

			for (size_t i = 0; i < server.client.size() ; i++) // in this i <= Just to evet loop
			{
                if (server.client.size() != 0)
                {
				    client &client = server.client[i];
				    if (FD_ISSET(client.fd_client, &readable) && client.check == 0)
				    {
                        std::cout<<"read request hna ******\n";
                        client.resp = client.request_client->read_reqwest(client);
				        fcntl(client.fd_client, F_SETFL, O_NONBLOCK);
                        // client.check = 1;
				    }
				    else if (FD_ISSET(client.fd_client, &writable) && client.check == 1)
                    {
                        if (client.resp > 0){
                            std::cout<<"static code *-*"<<std::endl;
                            send(client.fd_client, GenerateResponseFromStatusCode(client.resp).c_str(), GenerateResponseFromStatusCode(client.resp).size(), 0);
                        }
                        else if (client.resp == 0)
                        {
                            std::cout << "\n\n************************************************************ SWITCH TO RESPNSE PART ************************************************************\n";
                            Response response;
                            std::string res = response.CreatResponse(server, *client.request_client);
                                std::cout << "\n***** Response ***** \n" << res << "\n----------------------------------\n";
                            int sending = send(client.fd_client, res.c_str(), res.size(), 0);
                            std::cout << "I SEND RESP TO THIS USER: " << client.fd_client << "\nSENDING: " <<  sending << std::endl;
                            std::cout << "\n###################################################################################################################################################\n\n";
                            // send(client.fd_client, GenerateResponseFromStatusCode(404).c_str(), GenerateResponseFromStatusCode(404).size(), 0);
                            client.pr = 1; /// change with client_status_life
                            // client.pr = send_video(client);   
                        }
                    }
                    if (client.resp == -1 || client.resp > 0 || client.pr) 
                    {
                        std::cout<<"client "<<client.fd_client<<" dropped succesfolly"<<std::endl;
                        close(client.fd_client);
                        FD_CLR(client.fd_client, &server::current);
                        close(client.readFd);
                        server.client.erase(server.client.begin() + i);
                        server::maxfd--;

                    }  
                }
			}
		}

    }
   
}

