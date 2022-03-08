//https://github.com/Iidkhebb/Exam-Rank03/blob/master/micro-paint/micro_paint.c

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#define ARGS_ERR "Error: argument\n"
#define FILE_OCC "Error: Operation file corrupted\n"
FILE *file;
char **zone;
int bg_with, bg_high, ret;
char bg_c, rect_c, type;
float rect_with, rect_high, rect_pos_with, rect_pos_high;

int ft_putstr(char *s)
{
	int i = 0;
	while (s[i])
	{
		write(1, &s[i], 1);
		i++;
	}
	return 1;
}

int draw(void)
{
	if ((type != 'r' && type != 'R') || rect_high <= 0 || rect_with <= 0)
		return -1;
	for (int y = 0; y < bg_high; y++)
	{
		for (int x = 0; x < bg_with; x++)
		{
			bool cond1 = !(x < rect_pos_with || y < rect_pos_high || y > (rect_pos_high + rect_high) || x > (rect_pos_with + rect_with));
			bool cond2 = x - rect_pos_with < 1 || y - rect_pos_high < 1 || (rect_pos_high + rect_high) - y < 1 || (rect_pos_with + rect_with) - x < 1;
			if ((cond1) && ((cond2 && type == 'r') || type == 'R'))
				zone[y][x] = rect_c;
		}
	}
	return 0;
}

int main(int ac, char **av)
{
	if (ac != 2)
		return (ft_putstr(ARGS_ERR));
	file = fopen(av[1], "r");
	if (!(file))
		return (ft_putstr(FILE_OCC));
	ret = fscanf(file, "%d %d %c\n", &bg_with, &bg_high, &bg_c);
	if (ret != 3 || bg_high <= 0 || bg_high > 300 || bg_with <= 0 || bg_with > 300)
		return(ft_putstr(FILE_OCC));

	zone = calloc(bg_with, sizeof(char *));
	for (int y = 0; y < bg_high; y++)
	{
		zone[y] = calloc(bg_with + 1, 1);
		memset(zone[y], bg_c, bg_with);
	}

	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &type, &rect_pos_with, &rect_pos_high, &rect_with, &rect_high, &rect_c)) != -1)
	{
		if (ret != 6 || draw() == -1)
			return (ft_putstr(FILE_OCC));
	}
	for (int i = 0; i < bg_high; i++)
	{
		ft_putstr(zone[i]);
		ft_putstr("\n");
	}
	return 0;
}