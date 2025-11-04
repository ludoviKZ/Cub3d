#include "Cub3D.h"

static void setup_dda_extra(t_vars *vars, t_raycast *r);
static void setup_dda_extra_extra(t_raycast *r);

/* Setup DDA and perform stepping until a wall is hit or the ray leaves
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

/* Compute absolute delta distance for a ray direction axis without using
   the conditional operator. Variables declared first then assigned. */
double compute_delta_dist(double rayDir)
{
	double  res;

	if (rayDir == 0)
		res = 1e30;
	else
		res = fabs(1.0 / rayDir);
	return (res);
}

/* Compute perpendicular distance and hit pixel coordinates, then render
   the wall slice. Declared variables first, then assigned. */
void	compute_hit_and_pixels(t_vars *vars, t_raycast *r)
{
	double  perpWallDist;
	double  wallDistPixels;
	int	 hit_px;
	int	 hit_py;

	if (r->hit)
	{
		if (r->side == 0)
			perpWallDist = (r->mapX - r->posX + (1 - r->stepX) / 2.0) / (r->rayDirX == 0 ? 1e-6 : r->rayDirX);
		else
			perpWallDist = (r->mapY - r->posY + (1 - r->stepY) / 2.0) / (r->rayDirY == 0 ? 1e-6 : r->rayDirY);
	}
	else
		perpWallDist = (double)r->max_length / (double)vars->cell_size;
	r->perpWallDist = perpWallDist;
	wallDistPixels = perpWallDist * (double)vars->cell_size;
	r->wallDistPixels = wallDistPixels;

	hit_px = r->px + (int)(r->rayDirX * wallDistPixels);
	hit_py = r->py + (int)(r->rayDirY * wallDistPixels);
	r->hit_px = hit_px;
	r->hit_py = hit_py;

	draw_3D_wall_slice(vars, r->ray_index, r->px, r->py, r->hit_px, r->hit_py, r->ray_angle);
}

static void setup_dda_extra(t_vars *vars, t_raycast *r)
{
	setup_dda_extra_extra(r);
	while (!r->hit)
	{
		if (r->sideDistX < r->sideDistY)
		{
			r->sideDistX += r->deltaDistX;
			r->mapX += r->stepX;
			r->side = 0;
		}
		else
		{
			r->sideDistY += r->deltaDistY;
			r->mapY += r->stepY;
			r->side = 1;
		}
		if (r->mapY < 0 || r->mapY >= r->map_rows)
			break;
		int row_len = ft_strlen(vars->map[r->mapY]);
		if (r->mapX < 0 || r->mapX >= row_len)
			break;
		if (vars->map[r->mapY] && vars->map[r->mapY][r->mapX] == '1')
			r->hit = 1;
		if (vars->map[r->mapY] && vars->map[r->mapY][r->mapX] == '1')
			break;
	}
}

static void setup_dda_extra_extra(t_raycast *r)
{
	r->hit = 0;
	r->side = 0;
}
