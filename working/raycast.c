/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 14:04:30 by kbossio           #+#    #+#             */
/*   Updated: 2026/01/06 17:24:18 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

void	setup_dda(t_vars *vars, t_raycast *r)
{
	r->posx = (double)r->px / (double)vars->cell_size;
	r->posy = (double)r->py / (double)vars->cell_size;
	r->mapx = (int)r->posx;
	r->mapy = (int)r->posy;
	r->deltadistx = compute_delta_dist(r->raydirx);
	r->deltadisty = compute_delta_dist(r->raydiry);
	if (r->raydirx < 0)
		r->stepx = -1;
	if (r->raydirx < 0)
		r->sidedistx = (r->posx - r->mapx) * r->deltadistx;
	else
	{
		r->stepx = 1;
		r->sidedistx = (r->mapx + 1.0 - r->posx) * r->deltadistx;
	}
	if (r->raydiry < 0)
		r->stepy = -1;
	if (r->raydiry < 0)
		r->sidedisty = (r->posy - r->mapy) * r->deltadisty;
	else
	{
		r->stepy = 1;
		r->sidedisty = (r->mapy + 1.0 - r->posy) * r->deltadisty;
	}
	setup_dda_extra(vars, r);
}

void	compute_ray_direction(t_vars *vars, t_raycast *r, int index)
{
	(void)vars;
	r->ray_angle = r->start_angle + ((double)index * r->angle_step);
	r->rad = r->ray_angle * (M_PI / 180.0);
	r->raydirx = cos(r->rad);
	r->raydiry = sin(r->rad);
}

void	draw_field_of_view(t_vars *vars)
{
	t_raycast	r;
	int			i;

	if (!vars || !vars->player || !vars->map)
		return ;
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
	double	wallx;

	compute_vars_setup(vars, r, &wallx);
	if (r->side == 0)
	{
		if (r->raydirx > 0)
			vars->texture->tex_index = 3;
		else
			vars->texture->tex_index = 2;
	}
	else
	{
		if (r->raydiry > 0)
			vars->texture->tex_index = 1;
		else
			vars->texture->tex_index = 0;
	}
	if (vars->texture->tex_index == 1 || vars->texture->tex_index == 3)
		wallx = 1.0 - wallx;
	draw_3d_wall_slice(vars, r, wallx);
}

void	compute_vars_setup(t_vars *vars, t_raycast *r, double *wallX)
{
	double	rl;
	double	angle_diff;

	if (r->hit)
	{
		if (r->side == 0)
			rl = (r->mapx - r->posx + (1 - r->stepx) / 2.0)
				/ safe_denominator(r->raydirx);
		else
			rl = (r->mapy - r->posy + (1 - r->stepy) / 2.0)
				/ safe_denominator(r->raydiry);
	}
	else
		rl = (double)r->max_length / (double)vars->cell_size;
	r->hit_px = r->px + (int)round(r->raydirx * rl * (double)vars->cell_size);
	r->hit_py = r->py + (int)round(r->raydiry * rl * (double)vars->cell_size);
	angle_diff = r->rad - (double)vars->player->dir * (M_PI / 180.0);
	r->perpwalldist = rl * cos(angle_diff);
	r->walldistpixels = r->perpwalldist * (double)vars->cell_size;
	if (r->side == 0)
		*wallX = r->posy + rl * r->raydiry;
	else
		*wallX = r->posx + rl * r->raydirx;
	*wallX -= floor(*wallX);
}
