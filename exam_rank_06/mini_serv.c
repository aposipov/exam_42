#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <netinet/in.h>

int next_id = 0, max = 0, id[65536];
fd_set active, rdRead, rdWrite;
char str[42 * 4096], rdBuf[42 * 4096], wrBuf[42 * 4097];

void error(char *msg)
{
	write(2, msg, strlen(msg));
	exit(1);
}

void send_all(int cs)
{
	for (int i = 0; i <= max;i++)
		if(i!=cs && FD_ISSET(i,&rdWrite))
			send(i, wrBuf, strlen(wrBuf), 0);
}

int main(int ac, char **av)
{
	if(ac<2)
		error("Wrong number of arguments\n");
	FD_ZERO(&active);
	bzero(id, sizeof(id));
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);

    // ip port
	addr.sin_family = AF_INET; //
	addr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	addr.sin_port = htons(atoi(av[1])); //
  
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1 || bind(sockfd, (const struct sockaddr *)&addr, len) != 0
		|| listen(sockfd, 10) != 0)
		error("Fatal error\n");
	max = sockfd;
	FD_SET(sockfd, &active);
	while(1)
    {
		rdRead = rdWrite = active;
		if(select(max+1,&rdRead,&rdWrite,NULL,NULL)<0)
			continue;
		for (int s = 0; s <= max;s++)
			if(FD_ISSET(s,&rdRead))
            {
				if(s==sockfd)
                {
					int cli = accept(sockfd, (struct sockaddr *)&addr, &len);
					if(cli<0)
						continue;
					max = cli > max ? cli : max;
					id[cli] = next_id++;
					FD_SET(cli, &active);
					sprintf(wrBuf, "server: client %d just arrived\n", id[cli]);
					send_all(cli);
				}
                else
                {
					int ret = 1000, rdBuflen = 0;
					while(ret==1000 || rdBuf[rdBuflen-1]!='\n')
                    {
						ret = recv(s, rdBuf + rdBuflen, 1000, 0);
						if(ret<=0)
							break;
						rdBuflen += ret;
					}
					if(ret<=0)
                    {
						sprintf(wrBuf, "server: client %d just left\n", id[s]);
						send_all(s);
						FD_CLR(s, &active);
						close(s);
						break;
					}
					for (int i = 0, j = 0; i < rdBuflen;i++,j++)
                    {
						str[j] = rdBuf[i];
						if(str[j]=='\n')
                        {
							str[j] = 0;
							j = -1;
							sprintf(wrBuf, "client %d: %s\n", id[s], str);
							send_all(s);
						}
					}
				}
			}
	}
}
