#include <math.h>
#include "Cub3D.h"

int	init_player(t_vars *vars)
{
	int	y;
	int	x;

	y = 230;
	x = 200;
	vars->player = (t_player *)malloc(sizeof(t_player));
	if (!vars->player)
		return (-1);
	vars->player->x = x;
	vars->player->y = y;
	vars->player->dir = 0; // Initial direction
	/* Draw initial scene: map, player and field-of-view */
	redraw_scene(vars);
	draw_square(vars->mlx, vars->win, x, y, 5, 0xFF0000); // Draw player as a small red square
	draw_field_of_view(vars, 0xAAAAAA); // Draw field of view in light gray
	draw_line2D(vars->mlx, vars->win, x + 2, y + 2,
		x + 2 + (20 * cos(vars->player->dir * (M_PI / 180))),
		y + 2 + (20 * sin(vars->player->dir * (M_PI / 180))), 0x00FF00, 1); // Draw direction line in green
	return (0);
}
void	move(t_vars *vars, int ver, int side)
{
	if (!vars->player)
		return;
	/* Erase previous drawing by redrawing the scene background */
	redraw_scene(vars);
	if (validate_move(vars, ver, side) == 0)
	{
		vars->player->y += ver;
		vars->player->x += side;
	}
	/* Draw player and his direction and FOV */
	draw_square(vars->mlx, vars->win, vars->player->x, vars->player->y, 5, 0xFF0000);
	draw_line2D(vars->mlx, vars->win, vars->player->x + 2, vars->player->y + 2,
		vars->player->x + 2 + (20 * cos(vars->player->dir * (M_PI / 180))),
		vars->player->y + 2 + (20 * sin(vars->player->dir * (M_PI / 180))), 0x00FF00, 2);
	draw_field_of_view(vars, 0xAAAAAA);
}
void rotate(t_vars *vars, int angle)
{
	if (!vars->player)
		return;
	/* Redraw background, update direction and re-render player + FOV */
	redraw_scene(vars);
	vars->player->dir = (vars->player->dir + angle);
	if (vars->player->dir >= 360)
		vars->player->dir -= 360;
	else if (vars->player->dir < 0)
		vars->player->dir += 360;
	draw_line2D(vars->mlx, vars->win, vars->player->x + 2, vars->player->y + 2,
		vars->player->x + 2 + (20 * cos(vars->player->dir * (M_PI / 180))),
		vars->player->y + 2 + (20 * sin(vars->player->dir * (M_PI / 180))), 0x00FF00, 2);
	draw_field_of_view(vars, 0xAAAAAA);
	printf("Player direction: %d degrees\n", vars->player->dir);
}
int validate_move(t_vars *vars, int ver, int side)
{
	int new_x;
	int new_y;

	if (!vars || !vars->player)
		return (-1);
	new_x = vars->player->x + side;
	new_y = vars->player->y + ver;
	// Check pixel boundaries
	if (new_x < 0 || new_x >= MAP_WIDTH || new_y < 0 || new_y >= MAP_HEIGHT)
		return (-1);
	if ((new_x + 3) < 0 || (new_x + 3) >= MAP_WIDTH || (new_y + 3) < 0 || (new_y + 3) >= MAP_HEIGHT)
		return (-1);
	// If there's no map loaded, disallow movement to avoid deref of NULL
	if (!vars->map)
		return (-1);
	// Determine map cell indices (assuming each cell is 30x30 pixels)

	// Count map rows
	int rows = 0;
	while (vars->map[rows])
		rows++;
	if (new_y / 30 < 0 || new_y / 30 >= rows)
		return (-1);
	// Get width of the target row and validate column
	int cols = ft_strlen(vars->map[new_y / 30]);
	if (new_x / 30 < 0 || new_x / 30 >= cols)
		return (-1);
	// Check for walls in the map
	if (vars->map[new_y / 30][new_x / 30] == '1')
		return (-1);
	if (vars->map[(new_y + 3) / 30][(new_x + 3) / 30] == '1')
		return (-1);
	return (0);
}

void draw_field_of_view(t_vars *vars, int color)
{
	if (!vars || !vars->player || !vars->map)
		return;

	/* Raycast parameters */
	int fov_angle = 60; /* degrees */
	int num_rays = 120; /* more rays = smoother FOV */
	int max_length = 400; /* maximum ray length in pixels */

	double start_angle = (double)vars->player->dir - ((double)fov_angle / 2.0);
	double angle_step = (double)fov_angle / (double)num_rays;

	/* Use the player's visual center for ray origin */
	int px = vars->player->x + 2;
	int py = vars->player->y + 2;

	for (int i = 0; i < num_rays; ++i)
	{
		double ray_angle = start_angle + (i * angle_step);
		double rad = ray_angle * (M_PI / 180.0);
		double dx = cos(rad);
		double dy = sin(rad);

		int hit_x = px;
		int hit_y = py;

		/* Step along the ray in pixel increments until we hit a wall or reach max length */
		for (int r = 0; r < max_length; ++r)
		{
			int sx = px + (int)(r * dx);
			int sy = py + (int)(r * dy);
			/* bounds check before using map indices */
			if (sy < 0 || sx < 0 || sy >= MAP_HEIGHT || sx >= MAP_WIDTH)
			{
				hit_x = sx;
				hit_y = sy;
				break;
			}
			int cell_y = sy / 30;
			int cell_x = sx / 30;
			if (vars->map[cell_y] && vars->map[cell_y][cell_x] == '1')
			{
				hit_x = sx;
				hit_y = sy;
				break;
			}
		}
		draw_line2D(vars->mlx, vars->win, px, py, hit_x, hit_y, color, 1);
	}
	return;
}
void check_line_collision(t_vars *vars, int x1, int y1, int *x2, int *y2)
{
	/* Legacy helper retained but simplified; current draw_field_of_view
	   uses its own pixel stepping raycast. This function will clamp the
	   end point to the first wall it encounters along the straight
	   Bresenham line between (x1,y1) and (*x2,*y2). */
	int dx = abs(*x2 - x1);
	int dy = abs(*y2 - y1);
	int sx = (x1 < *x2) ? 1 : -1;
	int sy = (y1 < *y2) ? 1 : -1;
	int err = dx - dy;

	while (1)
	{
		if (y1 / 30 >= 0 && x1 / 30 >= 0 && vars->map[y1 / 30] && vars->map[y1 / 30][x1 / 30] == '1')
		{
			*x2 = x1;
			*y2 = y1;
			return;
		}
		if (x1 == *x2 && y1 == *y2)
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
	return;
}
