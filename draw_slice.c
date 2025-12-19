/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_slice.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 13:45:48 by kbossio           #+#    #+#             */
/*   Updated: 2025/12/19 16:36:00 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "Cub3D.h"

int	slice_height_calc(t_vars *vars, double perpWallDist)
{
	double	corrected_dist_pixels;

	corrected_dist_pixels = perpWallDist * (double)vars->cell_size;
	if (corrected_dist_pixels < 0.0001)
		corrected_dist_pixels = 0.0001;
	if (corrected_dist_pixels < 1.0)
		corrected_dist_pixels = 1.0;
	return ((int)((MAP_HEIGHT * vars->wall_scale) / corrected_dist_pixels));
}

void	draw_3d_wall_slice(t_vars *vars, t_raycast *r, double wallX)
{
	int	slice_height;
	int	top;
	int	bottom;
	int	wall_color;

	slice_height = slice_height_calc(vars, r->perpwalldist);
	if (slice_height < 1)
		slice_height = 1;
	top = (MAP_HEIGHT / 2) - (slice_height / 2);
	bottom = top + slice_height - 1;
	if (top < 0)
		top = 0;
	if (bottom >= MAP_HEIGHT)
		bottom = MAP_HEIGHT - 1;
	wall_color = 0x808080;
	if (bottom >= top)
		draw_texture(vars, r->ray_index, slice_height, wallX);
	return ;
}
