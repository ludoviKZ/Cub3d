/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_extra.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 13:48:41 by kbossio           #+#    #+#             */
/*   Updated: 2026/01/06 17:22:59 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

double	compute_delta_dist(double rayDir)
{
	double	res;

	if (rayDir == 0)
		res = 1e30;
	else
		res = fabs(1.0 / rayDir);
	return (res);
}

double	safe_denominator(double v)
{
	if (v == 0.0)
		return (1e-6);
	return (v);
}

void	setup_dda_extra(t_vars *vars, t_raycast *r)
{
	setup_dda_extra_extra(r);
	while (!r->hit)
	{
		if (r->sidedistx < r->sidedisty)
		{
			r->sidedistx += r->deltadistx;
			r->mapx += r->stepx;
			r->side = 0;
		}
		else
		{
			r->sidedisty += r->deltadisty;
			r->mapy += r->stepy;
			r->side = 1;
		}
		if (r->mapy < 0 || r->mapy >= r->map_rows)
			break ;
		if (r->mapx < 0 || r->mapx >= ft_strlen(vars->map[r->mapy]))
			break ;
		if (vars->map[r->mapy] && vars->map[r->mapy][r->mapx] == '1')
			r->hit = 1;
		if (vars->map[r->mapy] && vars->map[r->mapy][r->mapx] == '1')
			break ;
	}
}

void	setup_dda_extra_extra(t_raycast *r)
{
	r->hit = 0;
	r->side = 0;
}
