class client
{
int read;
}

while(1)
{
fd_set example;
FD_ZERO(&example);
FD_SET(serverSocket, &example);
while (i < clientlist)
{
FD_SET(clientlist[i], &example);
i++;
}
select(max_socket + 1, example, x , x, x);
if(FD_ISSET(serverSocket, &example))
{
  int client = accept()
clientlist.push_back(client);
                 fd_set(client,current)
}
while (i < clientlist.size())
{
if(FD_ISSET(clientlist, &read))
{
int read = recv(socket_client, 1000)
}
if(FD_ISSET(clientlist[i], &write))
{
send
}

}
