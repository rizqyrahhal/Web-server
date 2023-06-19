 #include <iostream>

int main(int ac , char **av, NULL)
{
	std::cout << "Host" << std::endl;
	const char *command = "gcc -o bin";
	av[1] = "/webserv/test.cpp"
	execve(command, av[1], NULL); // complie
	execve("bin", "bin", NULL); // complie
	return  0;
}
