#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

int max_fd;
int arr_id[5000];
char *arr_str[5000];
char *message;

fd_set actual_fds, read_fds, write_fds;

char buff_read[1001];
char buff_send[100];

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

void fatal_error()
{
	write(2, "Fatal error\n", strlen("Fatal error\n"));
	exit (1);
}

void send_msg(int sd_from)
{
	for (int sd = 0; sd <= max_fd; sd++)
	{
		if (FD_ISSET(sd, &write_fds) && sd != sd_from)
		{
			send(sd, buff_send, strlen(buff_send), 0);
			if (message)
				send(sd, message, strlen(message), 0);
		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(2, "Wrong number of arguments\n", strlen("Wrong number of arguments\n"));
		exit (1);
	}
	int sockfd, connfd;
	socklen_t len;
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1)
		fatal_error(); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(argv[1])); 
  
	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		fatal_error();
	if (listen(sockfd, 10) != 0)
		fatal_error();
	len = sizeof(cli);
	max_fd = sockfd;
	FD_ZERO(&actual_fds);
	FD_SET(sockfd, &actual_fds);
	int newId = 0;
	while(1)
	{
		read_fds = write_fds = actual_fds;
		if (select(max_fd + 1, &read_fds, &write_fds, NULL, NULL) <= 0)
			continue;
		if (FD_ISSET(sockfd, &read_fds))
		{
			connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
			if (connfd < 0)
				fatal_error();
			FD_SET(connfd, &actual_fds);
			max_fd = sockfd > max_fd ? sockfd : max_fd;
			arr_id[connfd] = newId++;
			arr_str[connfd] = NULL;
			sprintf(buff_send, "server: client %d just arrived\n", arr_id[connfd]);
			send_msg(connfd);
			continue;
		}
		for (int sd = 0; sd <= max_fd; sd++)
		{
			if (FD_ISSET(sd, &read_fds) && sd != sockfd)
			{
				int count = recv (sd, buff_read, 1000, 0);
				if (count <= 0)
				{
					sprintf(buff_send, "server: client %d just left\n", arr_id[sd]);
					send_msg(sd);
					FD_CLR(sd, &actual_fds);
					close (sd);
					if (arr_str[sd] != NULL)
						free(arr_str[sd]);
					break;
				}
				buff_read[count] = '\0';
				arr_str[sd] = str_join(arr_str[sd], buff_read);
				message = NULL;
				while(extract_message(&arr_str[sd], &message))
				{
					sprintf(buff_send, "client %d: ", arr_id[sd]);
					send_msg(sd);
				}
				if (message != NULL)
					free(message);
				message = NULL;
			}
		}
	}
}
