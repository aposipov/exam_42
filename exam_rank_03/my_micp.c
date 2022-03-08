
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ft_error(int error)
{
	if (error == 1)
		write(1, "Error: argument\n", 16);
	if (error == 2)
		write(1, "Error: Operation file corrupted\n", 32);
	return(1);
}

int main(int argc, char **argv)
{
	FILE *fd;
	int sf = 0, w = 0, h = 0, x, y, i = 0;
	float xtl, ytl, xbr, ybr;
	char	bgc, Rr, bgc2;
	char *canva;

	if (argc != 2)
		return(ft_error(1));
	if (!(fd = fopen(argv[1], "r")) || (sf = fscanf(fd, "%d %d %c\n", &w, &h,
													&bgc)) != 3 || w
													         < 1 || w > 300 || h < 1 || h > 300 || !(canva = malloc(sizeof(char) * w * h)))
		return (ft_error(2));
//	if (!(fd = fopen(argv[1], "r")))
//		return(ft_error(2));
//	if ((sf = fscanf(fd, "%d %d %c", &w, &h, &bgc)) != 3)
//	{
//		printf("%d\n", sf);
//		return(ft_error(2));
//	}
//	if (w < 1 || w > 300 || h < 1 || h > 300)
//		return(ft_error(2));
//	if (!(canva = malloc(sizeof(char) * w * h)))
//		return(ft_error(2));
	memset(canva, bgc, w * h);
	while ((sf = fscanf(fd, "%c %f %f %f %f %c\n", &Rr, &xtl, &ytl, &xbr,
						&ybr, &bgc2)) == 6)
	{
		printf("%d\n", sf);
		write (1, "test\n", 5);
		if ((Rr != 'r' && Rr != 'R') || xbr < 1 || ybr < 1)
		{
			write (1, "test_brake\n", 11);
			break;
		}
		xbr += xtl;
		ybr += ytl;
		if (xtl >= 0)
		{
			if (xtl - (int)xtl != 0)
				xtl += 1;
		}
		if (ytl >= 0)
		{
			if (ytl - (int)ytl != 0)
				ytl += 1;
		}
		y = 0;
		while (y != h)
		{
			x = 0;
			while (x != w)
			{
				if ( x >= (int)xtl && x <= (int)xbr && y >= (int)ytl &&
				y <= (int)ybr)
				{
					if (Rr == 'R')
						canva[y * w + x] = bgc2;
					else if (x == (int)xtl || x == (int)xbr || y == (int)ytl
					|| y == (int)ybr)
						canva[y * w + x] = bgc2;
				}
				x++;
			}
			y++;
		}

		write (1, "test2\n", 5);
	}


	if (sf != -1)
	{
		fclose(fd);
		free(canva);
		write (1, "test_-1\n", 8);
		return (ft_error(2));
	}
	while (i < h)
	{
		write (1, &canva[i* h], h);
		write(1, "\n", 1);
		i++;
	}

	//printf("%d\n", sf);
	write (1, "test\n", 5);
	fclose(fd);
	free(canva);
	return(0);
}
