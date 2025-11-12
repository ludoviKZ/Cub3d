#include "libft/libft.h"
#include "minilibx/mlx.h"
#include "Cub3D.h"

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
	int mid = MAP_HEIGHT / 2;
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