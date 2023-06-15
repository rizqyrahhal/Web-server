#include "web_serv.hpp" 

int server::maxfd = 0;
fd_set server::current = server::initializer();

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
