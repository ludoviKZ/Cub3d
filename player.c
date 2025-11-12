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
	redraw_scene(vars);
	double rad = (vars->player->dir * M_PI) / 180.0;
	double dx_f = -cos(rad) * (double)ver;
	double dy_f = -sin(rad) * (double)ver;
	double dx_s = -sin(rad) * (double)side;
	double dy_s =  cos(rad) * (double)side;
	double delta_x = dx_f + dx_s;
	double delta_y = dy_f + dy_s;

	if (validate_move(vars, delta_y, delta_x) == 0)
	{
		vars->player->y += delta_y;
		vars->player->x += delta_x;
	}
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
int validate_move(t_vars *vars, double ver, double side)
{
	double new_x;
	double new_y;

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
	int cell_row = (int)(new_y / vars->cell_size);
	if (cell_row < 0 || cell_row >= rows)
		return (-1);
	int cols = ft_strlen(vars->map[cell_row]);
	int cell_col = (int)(new_x / vars->cell_size);
	if (cell_col < 0 || cell_col >= cols)
		return (-1);
	if (vars->map[cell_row][cell_col] == '1')
		return (-1);
	int cell_row2 = (int)((new_y + 3) / vars->cell_size);
	int cell_col2 = (int)((new_x + 3) / vars->cell_size);
	if (cell_row2 < 0 || cell_row2 >= rows || cell_col2 < 0 || cell_col2 >= cols)
		return (-1);
	if (vars->map[cell_row2][cell_col2] == '1')
		return (-1);
	return (0);
}
