/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 18:00:07 by kbossio           #+#    #+#             */
/*   Updated: 2025/11/18 17:37:35 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

unsigned int	get_pixel(t_texture *tex, int x, int y)
{
	char	*pixel;

	if (!tex || x < 0 || y < 0 || x >= tex->width || y >= tex->height)
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

void	helper_draw(t_vars *vars, int slice_height)
{
	void	*col_img;
	int		top;
	int		bottom;
	int		draw_top;
	int		draw_bottom;

	top = (MAP_HEIGHT / 2) - (slice_height / 2);
	bottom = top + slice_height - 1;
	/* visible draw range (clip to screen) */
	draw_top = top;
	if (draw_top < 0)
		draw_top = 0;
	draw_bottom = bottom;
	if (draw_bottom >= MAP_HEIGHT)
		draw_bottom = MAP_HEIGHT - 1;
	/* create a temporary 1px-wide image for this column */
	col_img = mlx_new_image(vars->mlx, 1, MAP_HEIGHT);
}

void	draw(t_vars *vars, int draw_top, int draw_bottom, int slice_height, int top, int bottom, double wallX, char *dst)
{
	int		i;
	double	tex_yf;
	int		tex_y;
	int		tex_x;

	i = draw_top;
	while (i <= draw_bottom)
	{
		if (slice_height > 1)
			tex_yf = ((double)(i - top) / (double)(slice_height - 1))
				* (double)(vars->texture->height - 1);
		else
			tex_yf = 0.0;
		tex_y = (int)(tex_yf + 0.5);
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= vars->texture->height)
			tex_y = vars->texture->height - 1;
		tex_x = (int)(wallX * (double)vars->texture->width);
		if (tex_x < 0)
			tex_x = 0;
		if (tex_x >= vars->texture->width)
			tex_x = vars->texture->width - 1;
		dst = vars->texture->col_addr + (i * vars->texture->col_line_len);
		*(unsigned int *)dst = get_pixel(vars->texture, tex_x, tex_y);
		i++;
	}
	draw_background(vars, top, bottom, dst);
}

void	draw_wall(t_vars *vars, int screen_x, int slice_height, double wallX)
{
	void	*col_img;
	int		top;
	int		bottom;
	char	*dst;
	int		draw_top;
	int		draw_bottom;

	if (!vars->texture || !vars->texture->addr)
		return ;
	top = (MAP_HEIGHT / 2) - (slice_height / 2);
	bottom = top + slice_height - 1;
	/* visible draw range (clip to screen) */
	draw_top = top;
	if (draw_top < 0)
		draw_top = 0;
	draw_bottom = bottom;
	if (draw_bottom >= MAP_HEIGHT)
		draw_bottom = MAP_HEIGHT - 1;
	/* create a temporary 1px-wide image for this column */
	col_img = mlx_new_image(vars->mlx, 1, MAP_HEIGHT);
	if (col_img)
	{
		vars->texture->col_addr = mlx_get_data_addr(col_img, 
				&vars->texture->col_bpp,
				&vars->texture->col_line_len, &vars->texture->col_endian);
		dst = NULL;
		draw(vars, draw_top, draw_bottom, slice_height, top, bottom, wallX, dst);
		mlx_put_image_to_window(vars->mlx, vars->win, col_img, screen_x, 0);
		mlx_destroy_image(vars->mlx, col_img);
	}
	return ;
}

void	draw_texture(t_vars *vars, int ray, int sh, double wallX, int tex_index)
{
	vars->texture->addr = mlx_get_data_addr(vars->texture->img[tex_index],
			&vars->texture->bits_per_pixel,
			&vars->texture->line_length,
			&vars->texture->endian);
	draw_wall(vars, ray, sh, wallX);
}
