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
        std::stringstream tmp1(vector[i]);
        tmp1 >> str;
        if (str == "location")
        {
            tmp1 >> str;
            if (str == "{")
            {
                std::cout<<"Error : location name not exist!"<<std::endl;
                exit(0);
            }
            location.name = str;
        }
        else if (str == "upload_pass")
        {
            tmp1 >> str;
            if (access(str.c_str(), F_OK) == -1) {
                std::cout<<"Error : upload_pass path in location not exist!"<<std::endl;
                exit(0);
            }
            location.upload_pass = str;
        }
        else if (str == "root")
        {
            tmp1 >> str;
            if (access(str.c_str(), F_OK) == -1) {
                std::cout<<"Error : root path in location "<< vector[0]<<" not exist!"<<std::endl;
                exit(0);
            }
            location.root = str;
        }
        else if (str == "autoindex"){
            tmp1 >> str;
            if (str != "on" && str != "off")
            {
                std::cout<<"Error : auto index not correct!!"<<std::endl;
                exit(0);
            }
            location.autoindex = str;
        }
        else if (str == "index")
        {
            tmp1 >> str;
            if (access(str.c_str(), F_OK) == -1) {
                std::cout<<"Error : index path in location not exist!"<<std::endl;
                exit(0);
            }
            location.index = str;
        }
        else if (str == "allow_methods"){
            while(tmp1 >> str)
            {
                location.allow_methods.push_back(str);
            }
            if (location.allow_methods.size() > 3)
            {
                std::cout<<"allow methods not correct---------"<<std::endl;
                exit(0);
            }
            if (location.allow_methods.size() < 1)
            {
                std::cout<<"you must have one method at least---------"<<std::endl;
                exit(0);
            }
            for (size_t i = 0; i < location.allow_methods.size(); i++)
            {
                if (location.allow_methods[i] != "GET" && location.allow_methods[i] != "POST" && location.allow_methods[i] != "DELETE")
                {
                    std::cout<<"allow methods not correct *******"<<std::endl;
                    exit(0);
                }
            }
            
        }
        else if (str == "error_pages")
        {
            int err;
            tmp1 >> err;
            tmp1 >> str;
            std::cout<<"hello from error pages"<<std::endl;
            if (access(str.c_str(), F_OK) == -1) {
                std::cout<<"Error : error page path int locations not exist!"<<std::endl;
                exit(0);
            }
            location.map_err_page.insert(std::make_pair(err, str));
        }
        else if(str == "cgi")
        {
            tmp1 >> str;
            std::string str1;
            tmp1 >> str1;
            if (access(str1.c_str(), F_OK) == -1) {
                std::cout<<"Error : cgi path int locations not exist!"<<std::endl;
                exit(0);
            }
            location.cgi.insert(std::make_pair(str, str1));
        }
        else if(str == "redirect")
        {
            int req;
            tmp1 >> req;
            if (req == 0)
            {
                std::cout<<"Error : redirect not correct !"<<std::endl;
                exit(1);
            }
            tmp1 >> str;
            if (str == "redirect")
            {
                std::cout<<"Error : redirect not correct !"<<std::endl;
                exit(1);
            }
            location.redirect.insert(std::make_pair(req, str));
            // if (location.redirect.size())
        }
    }
    server.locations.push_back(location);
}


void    parce_server(std::string line, global & global)
{   
    server server;
    int ps = 0;
    int loc = 0;
    int prt = 0;
    while(1)
    {
        std::string location;
        size_t pos = line.find("location ", ps);
        if (pos == std::string::npos && loc == 0)
        {
            std::cout<<"Error: no location found in server block!"<<std::endl;
            exit(0);
        }
        loc++;
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
            if (str != "localhost" && str != "127.0.0.1" && str != "10.11.4.4")
            {
                std::cout<<"Error : ip address not correct!"<<std::endl;
                exit(1);
            }
            server.ip_address = str;
        }
        if (str == "root")
        {
            tmp1 >> str;
            if (access(str.c_str(), F_OK) == -1) {
                std::cout<<"Error : path root not exist!"<<std::endl;
                exit(0);
            }
            server.root = str;
        }
        if (str == "index")
        {
            tmp1 >> str;
            if (access(str.c_str(), F_OK) == -1) {
                std::cout<<"Error : path index not exist!"<<std::endl;
                exit(0);
            }
            server.root = str;
        }

        else if (str == "port")
        {
            tmp1 >> str;
            if (prt == 0){
            server.port.erase(server.port.begin());
            prt++;
            }
            if (str.size() > 4){
                std::cout<<"Error: port not correct !"<<std::endl;
                exit(1);
            }
            std::vector<std::string>::iterator it = std::find(server.port.begin(), server.port.end(), str);
            if (it != server.port.end())
            {
                std::cout<<"you should have one port! "<<std::endl;
                exit(0);
            }
            server.port.push_back(str);
        }
        else if (str == "server_names")
        {
            tmp1 >> str;
            server._name = str;
        }
        else if (str == "client_body_size")
        {
            size_t size;
            tmp1 >> size;
            server.client_body_size = size;
        }
        else if (str == "error_pages")
        {
            int err;
            tmp1 >> err;
            tmp1 >> str;
            if (access(str.c_str(), F_OK) == -1) {
                std::cout<<str<<std::endl;
                std::cout<<"Error : path error page not exist!"<<std::endl;
                exit(0);
            }
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
    if (line.empty())
    {
        std::cout<<"Error: no config file found"<<std::endl;
        exit(0);
    }
    int acolad = 0;
    for (size_t i = 0; i < line.size(); i++)
    {
        if (line[i] == '{')
            acolad++;
        else if (line[i] == '}')
            acolad--;
    }
    if (acolad != 0)
    {
        std::cout<<"accolad not closed"<<std::endl;
        exit(1);
    }
    (void)global;
    int ps = 0;
    while(1)
    {
        std::string server_conf;
        size_t pos = line.find("};", ps);
        if (pos == std::string::npos) {
            break;
        }
        server_conf = line.substr(ps, pos + 2);
        line = line.substr(pos + 2);
        parce_server(server_conf, global);
    }
}