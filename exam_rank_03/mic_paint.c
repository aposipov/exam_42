/*https://github.com/codesshaman/Exam_Rank_03*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
int w, h;
char bg, **tab;

typedef struct draw {
	char t, c;
	float x, y, w, h;
}   Draw;

int error_msg(FILE *fd, int err)
{
	if (err == 2)
		write(1, "Error: Operation file corrupted\n", 32);
	else if (err == 1)
		write(1, "Error argument\n", 16);
	else{
		for(int i = 0; i < h; i++){
			write(1, tab[i], w);
			write(1, "\n", 1);
		}
	}
	if (fd)
		fclose(fd);
	return (err);
}

int in_rectangle(float x, float y, Draw *el)
{
	if (((x < el->x) || (el->x + el->w) < x) || (y < el->y) || (el->y + el->h
	        < y))
		return (0);
	if (x - el->x < 1.000 || (el->x + el->w) - x < 1.000 || (y - el->y < 1.0000) || (el->y + el->h) - y < 1.0000)
		return (2);
	return (1);
}

int main (int ac, char **av)
{
	FILE *fd;
	float rect;
	int res;
	Draw el;
	fd = NULL;
	if (ac != 2)
		return (error_msg(fd, 1));
	if ((fd = fopen(av[1], "r")))
	{
		if ((res = fscanf(fd,"%d %d %c", &w, &h, &bg)) ==3)
		{
			if (w> 0 && w <= 300 && h > 0 && h <= 300)
			{
				tab = malloc(h * sizeof(char *));
				for (int i = 0; i < h; i++)
				{
					tab[i] = malloc(w * sizeof(char));
					memset(tab[i], bg, w);
				}
				while(1)
				{
					res = fscanf(fd,"\n%c %f %f %f %f %c",&el.t, &el.x, &el.y, &el.w, &el.h, &el.c);
					if (res == -1)
						return (error_msg(fd, 0));
					if (res != 6 || el.w <= 0 || el.h<= 0 ||(el.t != 'r' && el.t != 'R'))
						break;
					for (int line = 0; line < h; line ++){
						for (int col = 0; col < w; col++){
							rect = in_rectangle(col, line , &el);
							if(el.t == 'r' && rect == 2)
								tab[line][col] = el.c;
							else if (el.t == 'R' && rect )
								tab[line][col] = el.c;
						}
					}
				}
			}
		}
	}
	return (error_msg(fd, 2));
}

