#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int	 get_next_line(char **line)
{
	int rd = 0;
	int i = 0;
	char ch;

	char *buffer = malloc(100000);
	*line = buffer;
	while((rd = read(0, &ch, 1)) > 0 && ch != '\n')
		buffer[i++] = ch;
	buffer[i] = '\0';
	return (rd);
}

int main()
{
	int r;
	char *line;

	line = NULL;
	printf("----------------\n");
	while((r = get_next_line(&line)) > 0)
	{
		printf("%s\n", line);
		free(line);
		line = NULL;
	}
	printf("%s", line);
	free(line);
	line = NULL;
	printf("------------------\n");
}