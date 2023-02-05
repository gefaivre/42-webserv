#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

char **ft_split(const char *str);

int main(int argc, char **av, char **env)
{
    pid_t pid;
    // int fd[2];


	char *header[] = {
	(char *) "SCRIPT_FILENAME=/mnt/nfs/homes/jbach/Documents/Ecole42New/fomr.php",
	(char *) "REQUEST_METHOD=GET",
	// (char *)"CONTENT_TYPE=application/x-www-form-urlencoded",
	// (char *)"CONTENT_LENGTH=60",
	(char *) "QUERY_STRING=name=gr&email=hello%40hello.com",
	(char *) "REDIRECT_STATUS=200",
	(char *) NULL
	};
	// std::string answer_form = "name=value";
	// std::string answer_form = "name=gr&email=hello%40hello.com&file=00\n";
	char *args[]= {(char *)"/usr/bin/php-cgi", (char *) "-f", (char *)"/mnt/nfs/homes/jbach/Documents/Ecole42New/test.php", NULL};
	// char *args[]= {(char *)"/usr/bin/php-cgi", NULL};
//    pipe(fd);
	// write(fd[1], answer_form.c_str(), answer_form.length());
	// close(fd[1]);
	if ((pid = fork()) < 0) 
	{
		perror("fork");
		return EXIT_FAILURE;
	} 
	else if (!pid) 
	{ 
		/* child */
		// dup2(fd[0], STDIN_FILENO);
		// close(fd[0]);
		execve(args[0], args, header);
		perror("exec");
		return EXIT_FAILURE;
	} 
	else 
	{ 
		/* parent */
		// close(fd[0]);
		while (waitpid(-1, NULL, WUNTRACED) != -1)
			;
	}

    return EXIT_SUCCESS;
}
