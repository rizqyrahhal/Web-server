#include "networking/web_serv.hpp"

int server::maxfd = 0;
fd_set server::current = server::initializer();
fd_set server::current2 = server::initializer();

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
