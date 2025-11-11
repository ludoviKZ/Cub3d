#include "Cub3D.h"

void setup_dda_extra(t_vars *vars, t_raycast *r);
static void setup_dda_extra_extra(t_raycast *r);
static double safe_denominator(double v);
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

/* Return a non-zero denominator for division. If v is 0, return a
   small epsilon to avoid division by zero. Uses an if-statement to
   comply with the subject restriction against the conditional operator. */
static double safe_denominator(double v)
{
	if (v == 0.0)
		return 1e-6;
	return v;
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
			perpWallDist = (r->mapX - r->posX + (1 - r->stepX) / 2.0) / safe_denominator(r->rayDirX);
		else
			perpWallDist = (r->mapY - r->posY + (1 - r->stepY) / 2.0) / safe_denominator(r->rayDirY);
	}
	else
		perpWallDist = (double)r->max_length / (double)vars->cell_size;
	r->perpWallDist = perpWallDist;
	wallDistPixels = perpWallDist * (double)vars->cell_size;
	r->wallDistPixels = wallDistPixels;
	 hit_px = r->px + (int)round(r->rayDirX * wallDistPixels);
	 hit_py = r->py + (int)round(r->rayDirY * wallDistPixels);
	r->hit_px = hit_px;
	r->hit_py = hit_py;

	/* Pass perpendicular distance (in map cells) to drawing code so it can
	   compute slice height without recomputing or using integer hit points. */
	draw_3D_wall_slice(vars, r->ray_index, r->px, r->py, r->perpWallDist, r->ray_angle);
}

void setup_dda_extra(t_vars *vars, t_raycast *r)
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
