#include "libft/libft.h"
#include "minilibx/mlx.h"
#include "Cub3D.h"

void draw_square(void *mlx_ptr, void *win_ptr, int x0, int y0, int size, int color)
{
    int x;
    int y;

    y = 0;
    while (y < size - 1)
    {
        x = 0;
        while (x < size - 1)
        {
            mlx_pixel_put(mlx_ptr, win_ptr, x0 + x, y0 + y, color);
            x++;
        }
        y++;
    }
}
int draw_square2D(char **map, int size[], void *mlx_ptr, void *win_ptr)
{
	int x;
	int y;
	int square_size;

	square_size = 30; // Define the size of each square
	y = 0;
	while (y < size[1])
	{
		x = 0;
		while (x < size[0])
		{
			if (map[y][x] == '0' || map[y][x] == 'N' || map[y][x] == 'S' || map[y][x] == 'W' || map[y][x] == 'E')
				draw_square(mlx_ptr, win_ptr, x * square_size, y * square_size, square_size, 0xFFFFFF);
			else if (map[y][x] == '1')
				draw_square(mlx_ptr, win_ptr, x * square_size, y * square_size, square_size, 0x808080);
			x++;
		}
		y++;
	}
	return 0;
}

/* Redraw the entire 2D scene (map). This helper computes the map size from
   vars->map and calls draw_square2D so callers can erase previous lines and
   re-render the map before drawing player and FOV. */
void	redraw_scene(t_vars *vars)
{
	int rows = 0;
	int cols = 0;
	int size[2];

	if (!vars || !vars->map)
		return;
	while (vars->map[rows])
		rows++;
	if (rows > 0)
		cols = ft_strlen(vars->map[0]);
	size[0] = cols;
	size[1] = rows;
	printf("Redrawing scene with map size %d x %d\n", cols, rows);
	draw_square2D(vars->map, size, vars->mlx, vars->win);
}
void draw_line2D(void *mlx_ptr, void *win_ptr, int x1, int y1, int x2, int y2, int color, int thickness)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;
	int err = dx - dy;
    
	/* Determine the square offset to draw a filled block for thickness
	   start will be negative or zero; we draw thickness x thickness pixels
	   centered (approximately) on the line point. */
	int start = -(thickness / 2);
	int end = start + thickness - 1;

	while (1)
	{
		/* Draw a small filled square centered at (x1,y1) of size thickness */
		for (int oy = start; oy <= end; ++oy)
		{
			for (int ox = start; ox <= end; ++ox)
			{
				mlx_pixel_put(mlx_ptr, win_ptr, x1 + ox, y1 + oy, color);
			}	
		}

		if (x1 == x2 && y1 == y2)
			break;
		int err2 = err * 2;
		if (err2 > -dy)
		{
			err -= dy;
			x1 += sx;
		}
		if (err2 < dx)
		{
			err += dx;
			y1 += sy;
		}
	}
}
