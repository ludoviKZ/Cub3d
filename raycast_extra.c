#include "Cub3D.h"

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

double safe_denominator(double v)
{
	if (v == 0.0)
		return 1e-6;
	return v;
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

void setup_dda_extra_extra(t_raycast *r)
{
	r->hit = 0;
	r->side = 0;
}
