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
        std::string first = s.ip_address + s.port;
        std::string name = s.server_name;
        std::vector<server> ser = map[first];
        for (size_t i = 0; i < ser.size(); i++)
        {    
            if (ser[i].server_name == name) {
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
        //     std::cout<<"location name : "<< global.server[0].locations[i].name<<std::endl;
        // }
    }
    catch(const std::exception& e)
    {
        std::cout<<"ERROR: "<<e.what()<<std::endl;
    }
}










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
