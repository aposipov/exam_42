/*gvarys*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int error(int err)
{
	if (err == 1)
		write(1, "Error: argument\n", 16);
	if (err == 2)
		write(1, "Error: Operation file corrupted\n", 32);
	return (1);
}

int main(int argc, char **argv)
{
	FILE *file;
	float xtl, ytl, rad, dist;
	int x, y, width = 0, height = 0, scan = 0, i = 0;
	char Cc, bg, color;
	char *canva;

	if (argc != 2)
		return (error(1));
	if (!(file = fopen(argv[1], "r")))
		return (error(2));
	if ((scan = fscanf(file, "%d %d %c\n", &width, &height, &bg)) != 3)
		return (error(2));
	if (width < 1 || width > 300 || height < 1 || height > 300)
		return (error(2));
	if (!(canva = malloc(width * height)))
		return (error(2));

	memset(canva, bg, width * height);
	while ((scan = fscanf(file, "%c %f %f %f %c\n", &Cc, &xtl, &ytl, &rad, &color)) == 5)
	{
		if ((Cc != 'c' && Cc != 'C') || rad <= 0)
			break;
		y = 0;
		while (y != height)
		{
			x = 0;
			while (x != width)
			{
				dist = sqrtf(((float)x - xtl) * ((float)x - xtl) + ((float)y - ytl) * ((float)y - ytl));
				if (dist <= rad)
				{
					if (Cc == 'C')
						canva[width * y + x] = color;
					else if (dist + 1 > rad && Cc == 'c')
						canva[width * y + x] = color;
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
		write(1, &canva[i * height], width);
		write(1, "\n", 1);
		i++;
	}
	fclose(file);
	free(canva);
	return (0);
}