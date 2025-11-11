#include <math.h>
#include "Cub3D.h"

int	init_player(t_vars *vars)
{
	int	y;
	int	x;

	vars->cell_size = CELL_SIZE;
	y = vars->parsing->pl_y * vars->cell_size;
	x = vars->parsing->pl_x * vars->cell_size;
	vars->player = (t_player *)malloc(sizeof(t_player));
	if (!vars->player)
		return (-1);
	vars->player->x = x;
	vars->player->y = y;
	vars->player->dir = 0; // Initial direction
	vars->wall_scale = WALL_SCALE;
	vars->move_step = 20;
	redraw_scene(vars);
	draw_field_of_view(vars); // draw 3D projection across the screen
	return (0);
}
void	move(t_vars *vars, int ver, int side)
{
	if (!vars->player)
		return;
	/* Erase previous drawing by redrawing the scene background (clears to ceiling/floor) */
	redraw_scene(vars);
	 /* Interpret `ver` as forward/back amount and `side` as strafing amount
		 relative to the player's current direction (degrees). */
	 double rad = (double)vars->player->dir * (M_PI / 180.0);
	 /* forward vector */
	 double fx = cos(rad);
	 double fy = sin(rad);
	 /* right (strafe) vector is +90 degrees from forward */
	 double sx = cos(rad + (M_PI / 2.0));
	 double sy = sin(rad + (M_PI / 2.0));

	 int delta_x = (int)round(fx * (double)ver + sx * (double)side);
	 int delta_y = (int)round(fy * (double)ver + sy * (double)side);
	if (validate_move(vars, delta_y, delta_x) == 0)
	{
		vars->player->y += delta_y;
		vars->player->x += delta_x;
	}
	/* Re-render 3D view after movement */
	draw_field_of_view(vars);
}
void rotate(t_vars *vars, int angle)
{
	if (!vars->player)
		return;
	/* Redraw background, update direction and re-render 3D view */
	redraw_scene(vars);
	vars->player->dir = (vars->player->dir + angle);
	if (vars->player->dir >= 360)
		vars->player->dir -= 360;
	else if (vars->player->dir < 0)
		vars->player->dir += 360;
	draw_field_of_view(vars);
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
	if (new_x < 0 || new_x >= MAP_WIDTH || new_y < 0 || new_y >= MAP_HEIGHT)
		return (-1);
	if ((new_x + 3) < 0 || (new_x + 3) >= MAP_WIDTH || (new_y + 3) < 0 || (new_y + 3) >= MAP_HEIGHT)
		return (-1);
	if (!vars->map)
		return (-1);
	int rows = 0;
	while (vars->map[rows])
		rows++;
	if (new_y / vars->cell_size < 0 || new_y / vars->cell_size >= rows)
		return (-1);
	int cols = ft_strlen(vars->map[new_y / vars->cell_size]);
	if (new_x / vars->cell_size < 0 || new_x / vars->cell_size >= cols)
		return (-1);
	if (vars->map[new_y / vars->cell_size][new_x / vars->cell_size] == '1')
		return (-1);
	if (vars->map[(new_y + 3) / vars->cell_size][(new_x + 3) / vars->cell_size] == '1')
		return (-1);
	return (0);
}
