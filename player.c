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
	draw_square(vars->mlx, vars->win, x, y, 5, 0xFF0000); // Draw player as a small red square
	draw_line2D(vars->mlx, vars->win, x + 2, y + 2,
		x + 2 + (20 * cos(vars->player->dir * (M_PI / 180))),
		y + 2 + (20 * sin(vars->player->dir * (M_PI / 180))), 0x00FF00); // Draw direction line in green
	return (0);
}
void	move(t_vars *vars, int ver, int side)
{
	if (!vars->player)
		return;
	draw_square(vars->mlx, vars->win, vars->player->x, vars->player->y, 5, 0xFFFFFF); // Erase previous position
	draw_line2D(vars->mlx, vars->win, vars->player->x + 2, vars->player->y + 2,
		vars->player->x + 2 + (20 * cos(vars->player->dir * (M_PI / 180))),
		vars->player->y + 2 + (20 * sin(vars->player->dir * (M_PI / 180))), 0xFFFFFF); // Erase previous direction line
	if (validate_move(vars, ver, side) == 0)
	{
		vars->player->y += ver;
		vars->player->x += side;
	}
	draw_square(vars->mlx, vars->win, vars->player->x, vars->player->y, 5, 0xFF0000);
	draw_line2D(vars->mlx, vars->win, vars->player->x + 2, vars->player->y + 2,
		vars->player->x + 2 + (20 * cos(vars->player->dir * (M_PI / 180))),
		vars->player->y + 2 + (20 * sin(vars->player->dir * (M_PI / 180))), 0x00FF00);
}
void rotate(t_vars *vars, int angle)
{
	if (!vars->player)
		return;
	draw_line2D(vars->mlx, vars->win, vars->player->x + 2, vars->player->y + 2,
		vars->player->x + 2 + (20 * cos(vars->player->dir * (M_PI / 180))),
		vars->player->y + 2 + (20 * sin(vars->player->dir * (M_PI / 180))), 0xFFFFFF); // Erase previous direction line
	vars->player->dir = (vars->player->dir + angle);
	if (vars->player->dir >= 360)
		vars->player->dir -= 360;
	else if (vars->player->dir < 0)
		vars->player->dir += 360;
	draw_line2D(vars->mlx, vars->win, vars->player->x + 2, vars->player->y + 2,
		vars->player->x + 2 + (20 * cos(vars->player->dir * (M_PI / 180))),
		vars->player->y + 2 + (20 * sin(vars->player->dir * (M_PI / 180))), 0x00FF00);
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