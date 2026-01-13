/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 18:00:07 by kbossio           #+#    #+#             */
/*   Updated: 2026/01/06 17:01:37 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

unsigned int	get_pixel(t_texture *tex, int x, int y, int tex_index)
{
	char	*pixel;

	if (!tex || tex_index < 0 || tex_index >= 4)
		return (0);
	if (x < 0 || y < 0 || x >= tex->width[tex_index]
		|| y >= tex->height[tex_index])
		return (0);
	pixel = tex->addr + (y * tex->line_length + x * (tex->bits_per_pixel / 8));
	return (*(int *)pixel);
}

void	draw_background(t_vars *vars, int top, int bottom, char *dst)
{
	int		i;

	i = 0;
	while (i < top)
	{
		dst = vars->texture->col_addr + (i * vars->texture->col_line_len);
		*(unsigned int *)dst = (unsigned int)vars->parsing->colors[1];
		i++;
	}
	i = bottom + 1;
	while (i < MAP_HEIGHT)
	{
		dst = vars->texture->col_addr + (i * vars->texture->col_line_len);
		*(unsigned int *)dst = (unsigned int)vars->parsing->colors[0];
		i++;
	}
	return ;
}

void	draw(t_vars *v, double slice_height, double wallX)
{
	int		i;
	double	tex_yf;
	int		tex_y;
	int		tex_x;

	i = v->texture->draw_top;
	while (i <= v->texture->draw_bottom)
	{
		if (slice_height > 1)
			tex_yf = ((double)(i - v->texture->top) / (slice_height - 1))
				* (double)(v->texture->height[v->texture->tex_index] - 1);
		else
			tex_yf = 0.0;
		tex_y = (int)(tex_yf + 0.5);
		tex_x = (int)(wallX * (double)v->texture->width[v->texture->tex_index]);
		tex_set(&tex_y, &tex_x, v);
		v->texture->dst = v->texture->col_addr + (i * v->texture->col_line_len);
		*(unsigned int *)v->texture->dst = get_pixel(v->texture, tex_x,
				tex_y, v->texture->tex_index);
		i++;
	}
	draw_background(v, v->texture->top, v->texture->bottom, v->texture->dst);
}

void	draw_wall(t_vars *vars, int screen_x, int slice_height, double wallX)
{
	void	*col_img;

	if (!vars->texture || !vars->texture->addr)
		return ;
	vars->texture->top = (MAP_HEIGHT / 2) - (slice_height / 2);
	vars->texture->bottom = vars->texture->top + slice_height - 1;
	vars->texture->draw_top = vars->texture->top;
	if (vars->texture->draw_top < 0)
		vars->texture->draw_top = 0;
	vars->texture->draw_bottom = vars->texture->bottom;
	if (vars->texture->draw_bottom >= MAP_HEIGHT)
		vars->texture->draw_bottom = MAP_HEIGHT - 1;
	col_img = mlx_new_image(vars->mlx, 1, MAP_HEIGHT);
	if (col_img)
	{
		vars->texture->col_addr = mlx_get_data_addr(col_img,
				&vars->texture->col_bpp,
				&vars->texture->col_line_len, &vars->texture->col_endian);
		draw(vars, slice_height, wallX);
		mlx_put_image_to_window(vars->mlx, vars->win, col_img, screen_x, 0);
		mlx_destroy_image(vars->mlx, col_img);
	}
	return ;
}

void	draw_texture(t_vars *v, int ray, int sh, double wallX)
{
	v->texture->addr = mlx_get_data_addr(v->texture->img[v->texture->tex_index],
			&v->texture->bits_per_pixel,
			&v->texture->line_length,
			&v->texture->endian);
	draw_wall(v, ray, sh, wallX);
}
