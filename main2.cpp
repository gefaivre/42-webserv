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
    int fd[2];


	char *header[] = {
	// (char *)"name=hello",
	// (char *)"POST /test.php HTTP/1.1",
	// (char *)"Accept=text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9",
	// (char *)"Accept-Encoding=gzip, deflate, br",
	// (char *)"Accept-Language=en-US,en;q=0.9",
	// (char *)"Cache-Control=max-age=0",
	// (char *)"Connection=keep-alive",
	(char *) "SCRIPT_FILENAME=/mnt/nfs/homes/jbach/Documents/Ecole42New/test.php",
	(char *) "REQUEST_METHOD=POST",
	(char *)"CONTENT_TYPE=application/x-www-form-urlencoded",
	(char *)"CONTENT_LENGTH=60",
	(char *) "REDIRECT_STATUS=200",
	// (char *) "QUERY_STRING=name=gr&email=hello%40hello.com&file=00",
	// (char *)"CONTENT-TYPE=PHP 7.0.16",
	// (char *)"Host=localhost:8042",
	// (char *)"Origin=http://localhost:8042",
	// (char *)"Referer=http://localhost:8042/test.php",
	// (char *)"Sec-Fetch-Dest=document",
	// (char *)"Sec-Fetch-Mode=navigate",
	// (char *)"Sec-Fetch-Site=same-origin",
	// (char *)"Sec-Fetch-User=?1",
	// (char *)"Upgrade-Insecure-Requests=1",
	// (char *)"User-Agent=Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36",
	// (char *)"sec-ch-ua=\"Google Chrome\";v=\"107\", \"Chromium\";v=\"107\", \"Not=A?Brand\";v=\"24\"",
	// (char *)"sec-ch-ua-mobile=?0",
	// (char *)"sec-ch-ua-platform=\"Linux\"",
	// (char *) "GATEWAY_INTERFACE=CGI/1.1",
	// (char *) "SERVER_PROTOCOL=HTTP/1.1",
	// (char *) "SCRIPT_NAME=test.php",
	// (char *) "Content-Type="
	// (char *) "HTTP_REQUEST_METHOD=POST",
	// (char *) "REMOTE_HOST=127.0.0.1",
	// (char *) "SERVER_NAME=\"localhost\"",
	// (char *) "REQUEST_URI=test.php",
	// (char *)"\r\n",
	// (char *)"BODY=name=gr&email=hello%40hello.com&file=00\n",
	(char *) NULL
	};
	// std::string answer_form = "name=value";
	std::string answer_form = "name=gr&email=hello%40hello.com&file=00\n";
	char *args[]= {(char *)"/usr/bin/php-cgi", (char *) "-f", (char *)"/mnt/nfs/homes/jbach/Documents/Ecole42New/test.php", NULL};
	// char *args[]= {(char *)"/usr/bin/php-cgi", NULL};
   pipe(fd);
	write(fd[1], answer_form.c_str(), answer_form.length());
	close(fd[1]);
	if ((pid = fork()) < 0) 
	{
		perror("fork");
		return EXIT_FAILURE;
	} 
	else if (!pid) 
	{ 
		/* child */
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		execve(args[0], args, header);
		perror("exec");
		return EXIT_FAILURE;
	} 
	else 
	{ 
		/* parent */
		close(fd[0]);
		while (waitpid(-1, NULL, WUNTRACED) != -1)
			;
	}

    return EXIT_SUCCESS;
}
