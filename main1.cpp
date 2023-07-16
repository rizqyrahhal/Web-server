#include <iostream>
#include <string>
#include <vector>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/select.h>
#include <stdio.h>
#include <sstream>
#include <map>
#include <arpa/inet.h>
#include <algorithm>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

using  namespace std;

int add_it_to_body = 0;
size_t sizehex = 0; 
string string_to_add;

vector<string> chunks;


void parce_chunks(std::string body)
{
	size_t i = 0;
	// std::cout<<body.size()<<std::endl;
	string body2 = body;
	while (i < body.size())
	{
		printf("sizeH÷exa == %lu\n",sizehex);
		if (sizehex == 0)
		{
			int rn = body2.find("@@");
			std::cout<<"rn == "<<rn<< " read == " << body2 << std::endl;
			std::string hexa = body2.substr(0, rn);
            cout << "currStrHexa == " << hexa << endl;
			size_t size_hexa_string = hexa.size();
			std::stringstream hex;
			hex <<hexa;
			hex >> sizehex;
			std::cout<<"hexa == "<<sizehex<<std::endl;
			if (sizehex == 0)
				{
					std::cout<<"saliinaa mn chunked\n";
					return ;
				}
			body2 = body2.substr(rn + 2);
			
		}
		else if (body2.size() < sizehex && add_it_to_body == 0)
		{
            cout << "chunks ===|" <<body2 << "|===\n";  
            chunks.push_back(body2);
			sizehex -= body2.size();
			return ;
		}
		// tal hna mzyaaaan
		else if (body2.size() >= sizehex)
		{
            printf("here\n");
			if (add_it_to_body == 0)
			{
				printf("sizeTocut == %d | currBody = |%s|\n", sizehex, body2.c_str());
				std::string tomp = body2.substr(sizehex);
				int fnd = tomp.find("@@");
                cout << "body : |" << body2 << "|" << endl;
                cout << "tomp == |" << tomp << "|" << endl;
				if (fnd != -1)
				{
                    
					tomp = tomp.substr(2);
					int f2 = tomp.find("@@");
					if (f2 != -1)
					{
                        cout << "chunks ===|" <<body2.substr(0, sizehex) << "|===\n";  
                        chunks.push_back(body2.substr(0, sizehex));
						body2 = body2.substr(sizehex + 2);
						i += sizehex + 2;
						sizehex = 0;
					}
					else
					{    
                        cout << "chunks ===|" <<body2.substr(0, sizehex) << "|===\n";  
                        chunks.push_back(body2.substr(0, sizehex));
						// i += sizehex + 2;
						sizehex = 1;
						add_it_to_body = 1;
						string_to_add = tomp;
						return ;
					}
				}
				else 
				{
                    cout << "chunks ===|" <<body2.substr(0, sizehex) << "|===\n";  
                    chunks.push_back(body2.substr(0, sizehex));
					if (!tomp.empty())
					{
						string_to_add = tomp;
						add_it_to_body = 1;
						sizehex = 1;
					}
					return ;
				}
			}
			else{
				body2 = string_to_add + body2;
				if (body2[0] == '@')
					body2 = body2.substr(2);
				sizehex = 0;
				add_it_to_body = 0;
				i += string_to_add.size();
			}
		}
	}
}


int main() {

    int fd = open("text2.txt", O_RDONLY);
    char buffer[1025];
    while (1) {
        int sz = read(fd, buffer ,15) ;
        if (sz <= 0)
            break;
        string body = string(buffer, sz);
        parce_chunks(body);
        memset(buffer, 0, 1024);
        // break;
    }

    for (int i = 0; i < chunks.size() ;i++) {
        cout << "chunks ===|" << chunks[i] << "|===\n";
    }
}

/*
11
**********
*/