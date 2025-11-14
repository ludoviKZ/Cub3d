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

	square_size = CELL_SIZE; // Define the size of each square (use CELL_SIZE from header)
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
	/* Instead of drawing the 2D map, clear the window and paint a simple
	   ceiling (top half) and floor (bottom half) so the 3D projection can
	   occupy the full window width. */
	int mid = MAP_HEIGHT / 2;
	/* parsing->colors[0] = floor, parsing->colors[1] = ceiling
	   Use parsed integer colors when available; otherwise fall back to
	   default macros to avoid dereferencing NULL or uninitialized data. */
	int ceil_col = CEILING_COLOR;
	int floor_col = FLOOR_COLOR;
	if (vars && vars->parsing && vars->parsing->colors)
	{
		if (vars->parsing->colors[1] != -1)
			ceil_col = vars->parsing->colors[1];
		if (vars->parsing->colors[0] != -1)
			floor_col = vars->parsing->colors[0];
	}
	for (int y = 0; y < mid; ++y)
		draw_line2D(vars->mlx, vars->win, 0, y, MAP_WIDTH - 1, y, ceil_col, 1);
	for (int y = mid; y < MAP_HEIGHT; ++y)
		draw_line2D(vars->mlx, vars->win, 0, y, MAP_WIDTH - 1, y, floor_col, 1);
}
void draw_background(t_vars *vars, int screen_x, int top, int bottom, int ceil_col, int floor_col)
{
	/* Draw the ceiling (y=0..top-1) and the floor (y=bottom+1..MAP_HEIGHT-1)
	   for a single x column. We create a 1 x MAP_HEIGHT image, fill only the
	   ceiling and floor spans, then blit it once to avoid overwriting wall
	   pixels drawn separately.
	*/
	if (!vars || !vars->mlx || !vars->win)
		return;
	if (top < 0)
		top = 0;
	if (top > MAP_HEIGHT)
		top = MAP_HEIGHT;
	if (bottom < -1)
		bottom = -1;
	if (bottom >= MAP_HEIGHT)
		bottom = MAP_HEIGHT - 1;

	/* If neither ceiling nor floor needs drawing, skip */
	int need_ceil = (top > 0);
	int need_floor = (bottom < MAP_HEIGHT - 1);
	if (!need_ceil && !need_floor)
		return;

	void *col_img = mlx_new_image(vars->mlx, 1, MAP_HEIGHT);
	if (!col_img)
		return;

	int bpp = 0;
	int line_len = 0;
	int endian = 0;
	char *addr = mlx_get_data_addr(col_img, &bpp, &line_len, &endian);
	if (!addr)
	{
		mlx_destroy_image(vars->mlx, col_img);
		return;
	}

	//int pixel_size = bpp / 8;
	/* Fill ceiling span */
	if (need_ceil)
	{
		int y_end = top - 1;
		if (y_end >= MAP_HEIGHT)
			y_end = MAP_HEIGHT - 1;
		for (int y = 0; y <= y_end; ++y)
		{
			char *pixel = addr + (y * line_len);
			*(unsigned int *)pixel = (unsigned int)ceil_col;
		}
	}

	/* Fill floor span */
	if (need_floor)
	{
		int y_start = bottom + 1;
		if (y_start < 0)
			y_start = 0;
		for (int y = y_start; y < MAP_HEIGHT; ++y)
		{
			char *pixel = addr + (y * line_len);
			*(unsigned int *)pixel = (unsigned int)floor_col;
		}
	}
	vars->background = col_img;
	mlx_put_image_to_window(vars->mlx, vars->win, col_img, screen_x, 0);
}

void draw_line2D(void *mlx_ptr, void *win_ptr, int x1, int y1, int x2, int y2, int color, int thickness)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;
	int err = dx - dy;
	int start = -(thickness / 2);
	int end = start + thickness - 1;
	while (1)
	{
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

void line2D_vars_init(t_line2D *line, int x1, int y1, int x2, int y2, int thickness)
{
	line->dx = abs(x2 - x1);
	line->dy = abs(y2 - y1);
	line->sx = (x1 < x2) ? 1 : -1;
	line->sy = (y1 < y2) ? 1 : -1;
	line->start = -(thickness / 2);
	line->end = line->start + thickness - 1;
}