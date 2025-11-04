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
	vars->move_step = 5;
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
	if (validate_move(vars, ver, side) == 0)
	{
		vars->player->y += ver;
		vars->player->x += side;
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
	// Check pixel boundaries
	if (new_x < 0 || new_x >= MAP_WIDTH || new_y < 0 || new_y >= MAP_HEIGHT)
		return (-1);
	if ((new_x + 3) < 0 || (new_x + 3) >= MAP_WIDTH || (new_y + 3) < 0 || (new_y + 3) >= MAP_HEIGHT)
		return (-1);
	// If there's no map loaded, disallow movement to avoid deref of NULL
	if (!vars->map)
		return (-1);
	// Determine map cell indices (each cell is vars->cell_size x vars->cell_size pixels)

	// Count map rows
	int rows = 0;
	while (vars->map[rows])
		rows++;
	if (new_y / vars->cell_size < 0 || new_y / vars->cell_size >= rows)
		return (-1);
	// Get width of the target row and validate column
	int cols = ft_strlen(vars->map[new_y / vars->cell_size]);
	if (new_x / vars->cell_size < 0 || new_x / vars->cell_size >= cols)
		return (-1);
	// Check for walls in the map
	if (vars->map[new_y / vars->cell_size][new_x / vars->cell_size] == '1')
		return (-1);
	if (vars->map[(new_y + 3) / vars->cell_size][(new_x + 3) / vars->cell_size] == '1')
		return (-1);
	return (0);
}

/* draw_field_of_view was refactored into raycast.c / raycast_extra.c.
   The implementation was moved to keep player.c focused on player logic. */

void draw_3D_wall_slice(t_vars *vars, int ray, int px, int py, int hit_x, int hit_y, double ray_angle)
{
	// Calculate the distance to the wall
	double wall_dist = sqrt(pow(hit_x - px, 2) + pow(hit_y - py, 2));
	/* Prevent divide-by-zero and extremely large heights for tiny distances */
	if (wall_dist < 0.0001)
		wall_dist = 0.0001;

	 /* Correct fish-eye distortion: use the difference between this ray's angle
		 and the player's viewing angle (both in radians). */
	double player_dir_rad = (double)vars->player->dir * (M_PI / 180.0);
	double ray_rad = ray_angle * (M_PI / 180.0);
	double corrected_dist = wall_dist * cos(ray_rad - player_dir_rad);
	 /* Clamp corrected distance to avoid extremely large slice heights when
		 the computed distance is zero or very small (can happen near edges). */
	 if (corrected_dist < 1.0)
		  corrected_dist = 1.0;

	/* Apply configurable scale to make walls larger/smaller (runtime adjustable) */
	int slice_height = (int)((MAP_HEIGHT * vars->wall_scale) / corrected_dist);

	/* Clamp to screen height */
	if (slice_height > MAP_HEIGHT)
		slice_height = MAP_HEIGHT;
	if (slice_height < 0) 
		slice_height = 0;

	/* Compute top and bottom pixels of the vertical slice */
	int top = (MAP_HEIGHT / 2) - (slice_height / 2);
	int bottom = top + slice_height - 1;
	if (top < 0) top = 0;
	if (bottom >= MAP_HEIGHT) bottom = MAP_HEIGHT - 1;

	/* Clear above (ceiling) and below (floor) the slice so previous taller
	   slices don't leave pixels behind, then draw the wall slice once. */
	if (top > 0)
		draw_line2D(vars->mlx, vars->win, ray, 0, ray, top - 1, CEILING_COLOR, 1);

	 /* Use a neutral gray for walls so a rendering bug doesn't produce a
		 full-white screen; you can change this color later if desired. */
	 int wall_color = 0x808080;
	if (bottom >= top)
		draw_line2D(vars->mlx, vars->win, ray, top, ray, bottom, wall_color, 1);

	if (bottom < MAP_HEIGHT - 1)
		draw_line2D(vars->mlx, vars->win, ray, bottom + 1, ray, MAP_HEIGHT - 1, FLOOR_COLOR, 1);
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
		if (y1 / vars->cell_size >= 0 && x1 / vars->cell_size >= 0 && vars->map[y1 / vars->cell_size] && vars->map[y1 / vars->cell_size][x1 / vars->cell_size] == '1')
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
