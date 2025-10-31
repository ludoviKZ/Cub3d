#include <math.h>
#include "Cub3D.h"

int	init_player(t_vars *vars)
{
	int	y;
	int	x;

	/* Initialize runtime cell size from the compile-time macro. This lets
	   you change CELL_SIZE in the header and then rebuild to change the
	   starting value; it can also be adjusted at runtime via keybindings. */
	vars->cell_size = CELL_SIZE;

	y = vars->parsing->pl_y * vars->cell_size;
	x = vars->parsing->pl_x * vars->cell_size;
	vars->player = (t_player *)malloc(sizeof(t_player));
	if (!vars->player)
		return (-1);
	vars->player->x = x;
	vars->player->y = y;
	vars->player->dir = 0; // Initial direction
	/* Initialize runtime parameters on vars so they can be adjusted at runtime */
	vars->wall_scale = WALL_SCALE;
	vars->move_step = 5;
	/* Draw initial scene background (ceiling/floor) and 3D view */
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

void draw_field_of_view(t_vars *vars)
{
	if (!vars || !vars->player || !vars->map)
		return;

	/* Raycast parameters */
	int fov_angle = 60; /* degrees */
	/* Cast one ray per horizontal screen pixel so the 3D view fills the window */
	int num_rays = MAP_WIDTH;
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
		double rayDirX = cos(rad);
		double rayDirY = sin(rad);

		/* DDA setup: position in map-space (cells) */
	double posX = (double)px / (double)vars->cell_size;
	double posY = (double)py / (double)vars->cell_size;
		int mapX = (int)posX;
		int mapY = (int)posY;

		double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1.0 / rayDirX);
		double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1.0 / rayDirY);

		double sideDistX;
		double sideDistY;
		int stepX;
		int stepY;

		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (posX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - posX) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (posY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - posY) * deltaDistY;
		}

	int hit = 0;
	int side = 0;
	int rows = 0;
	while (vars->map[rows]) rows++;

		/* DDA loop: step through map grid until we hit a wall or leave bounds.
		   Use the actual row length for the current mapY when checking mapX to
		   avoid accessing past the end of a variable-length row. */
		while (!hit)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			/* Bail out if we've left the vertical bounds */
			if (mapY < 0 || mapY >= rows)
				break;
			/* Use the actual length of the current row for horizontal bounds */
			int row_len = ft_strlen(vars->map[mapY]);
			if (mapX < 0 || mapX >= row_len)
			{
				/* Ray left the map horizontally for this row */
				break;
			}
			if (vars->map[mapY] && vars->map[mapY][mapX] == '1')
			{
				hit = 1;
				break;
			}
		}

		double perpWallDist;
		if (hit)
		{
			if (side == 0)
				perpWallDist = (mapX - posX + (1 - stepX) / 2.0) / (rayDirX == 0 ? 1e-6 : rayDirX);
			else
				perpWallDist = (mapY - posY + (1 - stepY) / 2.0) / (rayDirY == 0 ? 1e-6 : rayDirY);
		}
		else
		{
				/* No hit: set far distance */
				perpWallDist = (double)max_length / (double)vars->cell_size;
		}

		/* Convert perpWallDist (in cells) to pixel distance and compute hit point */
	double wallDistPixels = perpWallDist * (double)vars->cell_size;
		int hit_px = px + (int)(rayDirX * wallDistPixels);
		int hit_py = py + (int)(rayDirY * wallDistPixels);

		/* Render the 3D vertical slice for this screen column */
		draw_3D_wall_slice(vars, i, px, py, hit_px, hit_py, ray_angle);
	}
	return;
}

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
