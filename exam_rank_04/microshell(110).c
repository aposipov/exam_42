Farid

#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#define READ_END 0
#define WRITE_END 1

typedef struct s_cmd {
	char	**argv;
	int		argc;
	int		pipe_fd[2];
	int		prev_pipe_fd;
	bool	is_pipe;
	bool	is_prev_pipe;
} t_cmd;


int init_cmd(t_cmd *cmd, int argc, char **argv, int i) {
	int start = i;
	while (i < argc && strcmp(argv[i], "|") != 0 && strcmp(argv[i], ";") != 0)
		++i;
	int len = i - start;
	if (len > 0) {
		cmd->argv = argv + start;
		cmd->argc = len;
		cmd->is_prev_pipe = cmd->is_pipe;
		cmd->is_pipe = i < argc && strcmp(argv[i], "|") == 0;
		cmd->prev_pipe_fd = cmd->pipe_fd[READ_END];
		argv[i] = NULL;
	}
	return (len);
}

size_t ft_strlen(const char *str) {
	size_t i = 0;
	while (str[i])
		++i;
	return i;
}

void ft_putstrfd(int fd, const char *str) {
	write(fd, str, ft_strlen(str));
}

void fatal() {
	ft_putstrfd(STDERR_FILENO, "error: fatal\n");
	exit(1);
}

void cd(t_cmd *cmd) {
	if (cmd->argc != 2)
		ft_putstrfd(STDERR_FILENO, "error: cd: bad arguments\n");
	else if (chdir(cmd->argv[1]) != 0) {
		ft_putstrfd(STDERR_FILENO, "error: cd: cannot change directory to ");
		ft_putstrfd(STDERR_FILENO, cmd->argv[1]);
		ft_putstrfd(STDERR_FILENO, "\n");
	}
}

void exec(t_cmd *cmd, char **envp) {
	if (cmd->is_pipe && pipe(cmd->pipe_fd) != 0)
			fatal();

	pid_t pid = fork();
	if (pid < 0)
		fatal();
	else if (pid == 0) {	
		if (cmd->is_pipe && (dup2(cmd->pipe_fd[WRITE_END], STDOUT_FILENO) < 0
				|| close(cmd->pipe_fd[READ_END]) != 0)
		)
				fatal();	
		if (cmd->is_prev_pipe && dup2(cmd->prev_pipe_fd, STDIN_FILENO) < 0)
				fatal();
		execve(cmd->argv[0], cmd->argv, envp);
		ft_putstrfd(STDERR_FILENO, "error: cannot execute ");
		ft_putstrfd(STDERR_FILENO, cmd->argv[0]);
		ft_putstrfd(STDERR_FILENO, "\n");
		exit(1);
	}
	else {
		if (cmd->is_pipe && close(cmd->pipe_fd[WRITE_END]) != 0)
			fatal();
		if (cmd->is_prev_pipe && close(cmd->prev_pipe_fd) != 0)
			fatal();
		waitpid(-1, 0, 0);
	}
}

int main(int argc, char **argv, char **envp)
{
	t_cmd	cmd;
	int		i = 0;

	cmd.is_pipe = false;
	while (++i < argc) {
		int start = i;
		i += init_cmd(&cmd, argc, argv, i);
		if (i > start) {
			if (strcmp(cmd.argv[0], "cd") == 0)
				cd(&cmd);
			else 
				exec(&cmd, envp);
		}
	}

	return 0;
}
