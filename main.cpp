#include "networking/web_serv.hpp"

int server::maxfd = 0;
fd_set server::current = server::initializer();
fd_set server::current2 = server::initializer();

void   check_servers(global & global)
{
    std::map< std::string, std::vector<server> > map;
    for(size_t i = 0; i < global.server.size(); i++)
    {
        server & s = global.server[i];
        std::string first = s.ip_address + s.port[0];
        std::cout<<"first == "<<first<<std::endl;
        std::string name = s._name;
        std::cout<<"name is : "<<name<<std::endl;
        std::vector<server> ser = map[first];
        for (size_t i = 0; i < ser.size(); i++)
        {
            std::cout<<"chhal mn mera dkhelt l had loop "<<std::endl;
            if (ser[i]._name == name) {
                std::cout << "this serverName already exist\n";
                exit(1);
            }
        }
        map[first].push_back(s);
        std::cout<<" ip + port "<<first<<std::endl;
        std::cout<<"map size : "<< map.size()<<std::endl;
        // std::cout<< "first server : "<< map[first][0].server_name<<std::endl;
    }
        creat_socket_and_bind(map);
        listen_new_connection(map);
        run_servers(map);
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
        check_servers(global);
        
        // for(size_t i = 0;i < global.server[0].locations.size();i++)
        // {
        //     std::cout<<"location name : "<< global.server[0].locations[i].allow_methods[1]<<std::endl;
        // }
    }
    catch(const std::exception& e)
    {
        std::cout<<"ERROR: "<<e.what()<<std::endl;
    }
}















// 













// #include "networking/web_serv.hpp"

// int server::maxfd = 0;
// fd_set server::current = server::initializer();
// fd_set server::current2 = server::initializer();

// int main(int ac, char **av)
// {
//     if (ac != 2)
//     {
//         std::cout<<"invalid argument !"<<std::endl;
//         return(0);
//     }
//     try
//     {
//         global global;
//         ft_parce_config(av, global);
//         creat_socket_and_bind(global);
//         listen_new_connection(global);
        
//         run_servers(global);
//         // for(size_t i = 0;i < global.server[0].locations.size();i++)
//         // {
//         //     std::cout<<"location name : "<< global.server[0].locations[i].name<<std::endl;
//         // }
//     }
//     catch(const std::exception& e)
//     {
//         std::cout<<"ERROR: "<<e.what()<<std::endl;
//     }
// }

















// else if (resp == 0)
//                     {
//                         std::cout << "\n\n************************************************************ SWITCH TO RESPNSE PART ************************************************************\n";
//                         Response response;
//                         ResponseReturned res = response.CreatResponse(server, *client.request_client);
//                         // ResponseReturned res(server, *client.request_client);
//                         // client.request_client->resp = res;
//                         // std::cout << "isFile: " << res.getIsFile() << std::endl << "BodyFile: " << res.getBody() << std::endl;
//                         // if (res.getHeaders().empty())
//                         //     std::cout << "\n***** Response1 ***** \n" << (res.readfile()) << "\n----------------------------------\n";
//                         // else
//                         std::cout << "\n***** Response2 ***** \n" << (res.getHeaders() + res.readfile()) << "\n----------------------------------\n";
//                         std::string chunck = res.GetChanckFromResponse(255);

//                         while(!chunck.empty()) {
//                             int sending = send(client.fd_client, chunck.c_str(), chunck.size(), 0);
//                             (void)sending;
//                             std::cout << "I SEND RESP TO THIS USER: " << client.fd_client << "\nSENDING: " <<  sending << std::endl;
//                             chunck = res.GetChanckFromResponse(255);
//                         }

//                         std::cout << "\n###################################################################################################################################################\n\n";
//                         //send correct response
//                         sen = true; /// change with client_status_life
//                     }