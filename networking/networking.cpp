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
        std::cout << "serv.fd_server " << it->second[0].fd_server  << std::endl;
        status = bind(it->second[0].fd_server, res->ai_addr, res->ai_addrlen);
        if (status == -1)
        {
            std::cout << "Failed to bind socket" << std::endl;
            close(it->second[0].fd_server);
            freeaddrinfo(res);
            exit(1);
        }
        freeaddrinfo(res);
        std::cout<<"---------finish----------"<<std::endl;
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

int    send_video(client & client)
{
    char readvideo[2025];
    if (client.p == 0)
    {

        client.readFd = open("/Users/araysse/Desktop/video.mp4", 0);
        if (client.readFd <= 0)
        {
            std::cout<<"open video failed"<<std::endl;
            return (1);
        }
        std::string header = "HTTP/1.1 200 OK\r\n"
                       "Content-Type: video/mp4\r\n"
                       "Content-Length: 32457473\r\n"
                       "Connection: closed\r\n\r\n";
        if (send(client.fd_client, header.c_str(), header.size(), 0) <= 0)
        {
            std::cout<<"send 1 salat******"<<std::endl;
            std::cout<<strerror(errno)<<"\n\n\n"<<std::endl;
            close(client.readFd);
            return (1);
        }
        client.p++;
        return (0);
    }
    int rd = read(client.readFd, &readvideo, 2024);
    if (rd <= 0)
    {
       std::cout<<"read salat"<<std::endl;
       close(client.readFd);
       return(1);
    }
    int snd = send(client.fd_client, &readvideo, rd, 0);
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
                std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  Pushing Clients @@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
				it->second[0].client.push_back(accept_new_connection(it->second[0]));
                std::cout << "server socket ---->" << it->second[0].client.size() << std::endl;
            }

			for (size_t i = 0; i < it->second[0].client.size() ; i++) // in this i <= Just to evet loop
			{
                if (it->second[0].client.size() != 0)
                {
				    client &client = it->second[0].client[i];
				    if (FD_ISSET(client.fd_client, &readable) && client.check == 0)
				    {

                        client.resp = client.request_client->read_reqwest(client, it->second);
                        std::cout << "client fd set :" << FD_ISSET(client.fd_client, &readable) << std::endl;
				        fcntl(client.fd_client, F_SETFL, O_NONBLOCK);
                        // client.check = 1;
				    }
				    else if (FD_ISSET(client.fd_client, &writable) && client.check == 1)
                    {
                        
                       if (client.resp > 0){
                            std::cout<<"static code *-*"<<client.resp<<std::endl;
                            if (send(client.fd_client, GenerateResponseFromStatusCode(client.resp, it->second[client.client_in_serv]).c_str(), GenerateResponseFromStatusCode(client.resp, it->second[client.client_in_serv]).size(), 0) <= 0)
                            {
                                client.pr = 1;
                            }
                        }
                        else if (client.resp == 0) {
                            std::cout << "I WORK ON THIS CLIENT: " << client.fd_client << std::endl;
                            std::cout << "\n\n************************************************************ SWITCH TO RESPNSE PART ************************************************************\n";
                            if (!client.generateResponseObject) {
                                Response response;
                                client.response_client = response.CreatResponse(it->second[client.client_in_serv], *client.request_client);
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

                            std::cout << "\n###################################################################################################################################################\n\n";
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



                        // else if (client.resp == 0)
                        // {
                        //     std::cout << "\n\n************************************************************ SWITCH TO RESPNSE PART ************************************************************\n";
                        //     std::string res;
                        //     if (client.siftna_response == true)
                        //     {
                        //     Response response;
                        //     res = response.CreatResponse(it->second[0], *client.request_client);
                        //     client.siftna_response = false;
                        //     }
                        //         // std::cout << "\n***** Response ***** \n" << res << "\n----------------------------------\n";
                        //     int sending = send(client.fd_client, res.c_str(), res.size(), 0);
                        //     std::cout << "I SEND RESP TO THIS USER: " << client.fd_client << "\nSENDING: " <<  sending << std::endl;
                        //     std::cout << "\n###################################################################################################################################################\n\n";
                        //     send(client.fd_client, GenerateResponseFromStatusCode(404).c_str(), GenerateResponseFromStatusCode(404).size(), 0);
                        //     client.pr = 1; /// change with client_status_life
                        //     // client.pr = send_video(client);
                        // }
/*


ParseRequest(client, servers, int idxClientInDefaultServer) {
    if (parsingHeader) {
        parseHeader();
        string host = getHoste();
        for (int i = 1; i < servers.size(); i++) {
            if (servers[i] == host) {
                servers[0].earese(begin() + idxClientInDefaultServer);
                servers[i].push_back(client);
                break;
            }
        }
    }
}

for (auto xs : map) {
    firstServer = xs.second[0];
    if (new client) {
        firstServer.client.push_back(newClient);
    }
    for (int i = 0; i < xs.second.size(); i++) {
        if (there is something to  read) {
            ParseRequest(client, servers);
        }
    }
}
*/