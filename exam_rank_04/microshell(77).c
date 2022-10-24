https://github.com/zlovsa/42-exam-rank/blob/master/42-exam-rank-04/microshell.c

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void error(char *s1, char *s2, int fatal)
{
	while(*s1)
		write(2, s1++, 1);
	while(*s2)
		write(2, s2++, 1);
	write(2, "\n", 1);
	if(fatal)
		exit(1);
}

void errf()
{
	error("error: fatal", "", 1);
}

int next(char **av, int i)
{
	while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
		i++;
	return i;
}

int exec(char **av, char **env, int from, int *fdi)
{
	int io[2] = {0, 1};
	int pipefd[2] = {0, 1};
	int to = next(av, from);
	int piped = av[to] && !strcmp(av[to], "|");
	if(piped && pipe(pipefd)<0)
		errf();
	av[to] = NULL;
	io[0] = *fdi;
	io[1] = pipefd[1];
	*fdi = pipefd[0];
	if(!strcmp(av[from],"cd"))
	{
		if(to-from!=2)
			error("error: cd: bad arguments", "", 0);
		else if(chdir(av[from+1]))
			error("error: cd: cannot change directory to ", av[from + 1], 0);
		return to;
	}
	int pid = fork();
	if(pid<0)
		errf();
	if(!pid)
	{
		if((io[0]&&dup2(io[0],0)==-1)
			||(io[1]!=1 && dup2(io[1],1)==-1)
			||(io[0] && close(io[0])==-1)
			||(io[1]!=1 && close(io[1])==-1)
			||(piped && close(pipefd[0])==-1))
			errf();
		execve(av[from], av + from, env);
		error("error: cannot execute ", av[from], 1);
	}
	waitpid(pid, NULL, 0);
	if((io[0] && close(io[0])==-1)
		||(io[1]!=1 && close(io[1])==-1))
		errf();
	return to;
}

int main(int ac, char **av, char **env)
{
	int fdi = 0;
	int i = 0;
	while (++i<ac)
		if (strcmp(av[i], ";"))
			i = exec(av, env, i, &fdi);
	return 0;
}
