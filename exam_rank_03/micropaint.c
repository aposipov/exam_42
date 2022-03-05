/*gvarys*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int error(int err)
{
	if (err == 1)
		write(1, "Error: argument\n", 16);
	if (err == 2)
		write (1, "Error: Operation file corrupted\n", 32);
	return (1);
}

int main(int argc, char **argv)
{
	FILE *file;
	float xtl, ytl, xbr, ybr;
	int x, y, i = 0, width = 0, height = 0, scan = 0;
	char Rr, bg, color;
	char *canva;

	if (argc != 2)
		return (error(1));
	if (!(file = fopen(argv[1], "r")) || (scan = fscanf(file, "%d %d %c\n", &width, &height, &bg)) != 3 || \
	width < 1 || width > 300 || height < 1 || height > 300 || !(canva = malloc(sizeof(char) * width * height)))
		return (error(2));
	memset(canva, bg, width * height);
	while((scan = fscanf(file, "%c %f %f %f %f %c\n", &Rr, &xtl, &ytl, &xbr, &ybr, &color)) == 6)
	{
		if ((Rr != 'r' && Rr != 'R') || xbr < 1 || ybr < 1)
			break ;
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
		while (y != height)
		{
			x = 0;
			while (x != width)
			{
				if (x >= (int)xtl && x <= (int)xbr && y >= (int)ytl && y <= (int)ybr)
				{
					if (Rr == 'R')
						canva[y * width + x] = color;
					else if (x == (int)xtl || x == (int)xbr || y == (int)ytl || y == (int)ybr)
						canva[y * width + x] = color;
				}
				x++;
			}
			y++;
		}
	}
	if (scan != -1)
	{
		fclose(file);
		free(canva);
		return (error(2));
	}
	while (i < height)
	{
		write(1, &canva[i * height], height);
		write(1, "\n", 1);
		i++;
	}
	fclose(file);
	free(canva);
	return (0);
}