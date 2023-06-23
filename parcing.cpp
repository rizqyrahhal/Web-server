#include "web_serv.hpp"

void    location_serv(std::string line, server & server)
{
    locations location;

    std::vector<std::string> vector;
    std::stringstream tmp(line);
    std::string str;
    while (std::getline(tmp, str))
    {
        vector.push_back(str);
    }
    for (size_t i = 0 ; i < vector.size(); i++){
        std::stringstream tmp1;
        tmp1 >> str;
        if (str == "location")
        {
            tmp1 >> str;
            location.name = str;
        }
        else if (str == "root")
        {
            tmp1 >> str;
            location.root = str;
        }
        else if (str == "autoindex"){
            tmp1 >> str;
            location.autoindex = str;
        }
        else if (str == "allow_methods"){
            while(tmp1 >> str)
            {
                location.allow_methods.push_back(str);
            }
            if (location.allow_methods.size() > 3)
            {
                std::cout<<"allow methods not correct"<<std::endl;
                exit(0);
            }
            for (size_t i = 0; i < location.allow_methods.size(); i++)
            {
                if (location.allow_methods[i] != "get" || location.allow_methods[i] != "post" || location.allow_methods[i] != "delete")
                 std::cout<<"allow methods not correct"<<std::endl;
                exit(0);
            }
            
        }
        else if(str == "cgi")
        {
            tmp1 >> str;
            std::string str1;
            tmp1 >> str1;
            location.cgi.insert(std::make_pair(str, str1));
        }
        else if(str == "redirect")
        {
            int req;
            tmp1 >> req;
            tmp1 >> str;
            location.redirect.insert(std::make_pair(req, str));
        }
    }
    server.locations.push_back(location);
}


void    parce_server(std::string line, global & global)
{   
    server server;
    int ps = 0;
    while(1)
    {
        std::string location;
        size_t pos = line.find("location ", ps);
        if (pos == std::string::npos) {
            break;
        }
        int pos1 = line.find("}", pos);
        location = line.substr(pos, pos1 - pos);
        ps = pos + 8;
        location_serv(location, server);

    }

    std::vector<std::string> vector;
    std::stringstream tmp(line);
    std::string str;
    while (std::getline(tmp, str))
    {
        vector.push_back(str);
    }
    
    for (size_t i = 0; i < vector.size(); i++)
    {
        std::stringstream tmp1(vector[i]);
        tmp1 >> str;
        if (str == "listen")
        {
            tmp1 >> str;
            server.ip_address = str;
        }
        else if (str == "port")
        {
            int port;
            tmp1 >> port;
            server.port = port;
        }
        else if (str == "server_name")
        {
            tmp1 >> str;
            server.server_name = str;
        }
        else if (str == "client_body_size")
        {
            int size;
            tmp1 >> size;
            server.client_body_size = size;
        }
        else if (str == "error_page")
        {
            int err;
            tmp1 >> err;
            tmp1 >> str;
            server.map_err_page.insert(std::make_pair(err, str));
        }
        else if (str == "location")
            break;
    }
    global.server.push_back(server);
}

void    ft_parce_config(char **av, global &global)
{
    std::string str;
    str = av[1];
    std::fstream file(av[1]);
    std::string line;
    if (file.is_open()){
        std::getline(file, line, '\0');
        file.close();
    }
    else{
        std::cout<<"Unable to open file. "<<std::endl;
        exit(0);
    }
    int ps = 0;
    while(1)
    {
        std::string server_conf;
        size_t pos = line.find("};", ps);
        if (pos == std::string::npos) {
            break;
        }
        server_conf = line.substr(ps, pos + 2);
        ps = pos + 2;
        parce_server(server_conf, global);

    }

}