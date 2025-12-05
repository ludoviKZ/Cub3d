#include "Cub3D.h"

/* Setup DDA and perform steppgiting until a wall is hit or the ray leaves
   the map. Variables declared first, assigned after. */
void	setup_dda(t_vars *vars, t_raycast *r)
{
	r->posX = (double)r->px / (double)vars->cell_size;
	r->posY = (double)r->py / (double)vars->cell_size;
	r->mapX = (int)r->posX;
	r->mapY = (int)r->posY;
	r->deltaDistX = compute_delta_dist(r->rayDirX);
	r->deltaDistY = compute_delta_dist(r->rayDirY);
	if (r->rayDirX < 0)
		r->stepX = -1;
	if (r->rayDirX < 0)
		r->sideDistX = (r->posX - r->mapX) * r->deltaDistX;
	else
	{
		r->stepX = 1;
		r->sideDistX = (r->mapX + 1.0 - r->posX) * r->deltaDistX;
	}
	if (r->rayDirY < 0)
		r->stepY = -1;
	if (r->rayDirY < 0)
		r->sideDistY = (r->posY - r->mapY) * r->deltaDistY;
	else
	{
		r->stepY = 1;
		r->sideDistY = (r->mapY + 1.0 - r->posY) * r->deltaDistY;
	}
	setup_dda_extra(vars, r);
}

/* Compute ray direction for a given ray index. Variables are declared at the
   top and assigned on separate lines to meet the requested style. */
void compute_ray_direction(t_vars *vars, t_raycast *r, int index)
{
	(void)vars;
	r->ray_angle = r->start_angle + ((double)index * r->angle_step);
	r->rad = r->ray_angle * (M_PI / 180.0);
	r->rayDirX = cos(r->rad);
	r->rayDirY = sin(r->rad);
}

/* Top-level draw function: iterates rays using a while loop (no for loops).
   It orchestrates the per-ray helpers placed across files. */
void draw_field_of_view(t_vars *vars)
{
	t_raycast   r;
	int         i;

	if (!vars || !vars->player || !vars->map)
		return;
	printf("Drawing field of view for player at (%.2f, %.2f) facing %d degrees\n",
	vars->player->x, vars->player->y, vars->player->dir);
	r.fov_angle = 60;
	r.num_rays = MAP_WIDTH;
	r.max_length = 400;
	r.start_angle = (double)vars->player->dir - ((double)r.fov_angle / 2.0);
	r.angle_step = (double)r.fov_angle / (double)r.num_rays;
	r.px = (int)(vars->player->x + 2.0);
	r.py = (int)(vars->player->y + 2.0);
	r.map_rows = 0;
	while (vars->map[r.map_rows])
		r.map_rows++;
	i = 0;
	while (i < r.num_rays)
	{
		r.ray_index = i;
		compute_ray_direction(vars, &r, i);
		setup_dda(vars, &r);
		compute_hit_and_pixels(vars, &r);
		i++;
	}
}
void	compute_hit_and_pixels(t_vars *vars, t_raycast *r)
{
	double wallX;
	int		tex_index;

	compute_vars_setup(vars, r, &wallX);
	if (r->side == 0)
	{
		if (r->rayDirX > 0)
			tex_index = 2; /* WE */
		else
			tex_index = 3; /* EA */
	}
	else
	{
		if (r->rayDirY > 0)
			tex_index = 1; /* SO */
		else
			tex_index = 0; /* NO */
	}
	if (tex_index == 1 || tex_index == 3)
		wallX = 1.0 - wallX; /* Flip for SO and EA walls */
	draw_3D_wall_slice(vars, r->ray_index, r->hit_px, r->hit_py, r->perpWallDist, r->ray_angle, wallX, tex_index);
}

void compute_vars_setup(t_vars *vars, t_raycast *r, double *wallX)
{
	double  rayLength;
	double  angle_diff;

	if (r->hit)
	{
		if (r->side == 0)
			rayLength = (r->mapX - r->posX + (1 - r->stepX) / 2.0) / safe_denominator(r->rayDirX);
		else
			rayLength = (r->mapY - r->posY + (1 - r->stepY) / 2.0) / safe_denominator(r->rayDirY);
	}
	else
		rayLength = (double)r->max_length / (double)vars->cell_size;
	r->hit_px = r->px + (int)round(r->rayDirX * rayLength * (double)vars->cell_size);
	r->hit_py = r->py + (int)round(r->rayDirY * rayLength * (double)vars->cell_size);
	angle_diff = r->rad - (double)vars->player->dir * (M_PI / 180.0);
	r->perpWallDist = rayLength * cos(angle_diff);
	r->wallDistPixels =  r->perpWallDist * (double)vars->cell_size;
	if (r->side == 0)
		*wallX = r->posY + rayLength * r->rayDirY;
	else
		*wallX = r->posX + rayLength * r->rayDirX;
	*wallX -= floor(*wallX);
}
