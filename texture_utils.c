/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:08:25 by lzorzit           #+#    #+#             */
/*   Updated: 2025/12/12 18:09:08 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

void	tex_set(int *tex_y, int *tex_x, t_vars *v)
{
	if (*tex_y < 0)
		*tex_y = 0;
	if (*tex_y >= v->texture->height)
		*tex_y = v->texture->height - 1;
	if (*tex_x < 0)
		*tex_x = 0;
	if (*tex_x >= v->texture->width)
		*tex_x = v->texture->width - 1;
}

void	helper_draw(t_vars *vars, int slice_height)
{
	void	*col_img;
	int		top;
	int		bottom;
	int		draw_top;
	int		draw_bottom;

	top = (MAP_HEIGHT / 2) - (slice_height / 2);
	bottom = top + slice_height - 1;
	draw_top = top;
	if (draw_top < 0)
		draw_top = 0;
	draw_bottom = bottom;
	if (draw_bottom >= MAP_HEIGHT)
		draw_bottom = MAP_HEIGHT - 1;
	col_img = mlx_new_image(vars->mlx, 1, MAP_HEIGHT);
}
