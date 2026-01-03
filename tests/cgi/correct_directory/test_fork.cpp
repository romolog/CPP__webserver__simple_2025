#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define CHILD_ 0
#define ERROR_ -1

int main(int argc, char** argv, char** env)
{

	int	cgi_sockets_out[2];
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, cgi_sockets_out) == -1)
		exit(EXIT_FAILURE);

	int pid = fork();
	if (pid == ERROR_)
		exit(EXIT_FAILURE);
	
	if (pid == CHILD_)
	{

		int check_dup2 = dup2(cgi_sockets_out[1], STDOUT_FILENO);
		if (check_dup2 == -1)
			exit(EXIT_FAILURE);

		char file_name[] = "script_relative_dir.cgi";
		char* ptr_file_name = file_name;
		char* exec_args[2] = {ptr_file_name, NULL};

		char chdir_path[] = "/home/rponomar/WebServer/9/test/cgi/correct_directory/";
		int check_chdir = chdir(chdir_path);
		if (check_chdir == ERROR_)
			exit(EXIT_FAILURE);

		execve(ptr_file_name, exec_args, env);
	}

	if (pid) // PARENT
	{
		int check_wait = wait(NULL);
		if (check_wait == ERROR_)
			exit(EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
