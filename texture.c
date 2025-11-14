/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 18:00:07 by kbossio           #+#    #+#             */
/*   Updated: 2025/11/14 18:27:06 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int	get_pixel(t_texture *tex, int x, int y)
{
	char	*pixel;

	if (!tex || x < 0 || y < 0 || x >= tex->width || y >= tex->height)
		return (0);
	pixel = tex->addr + (y * tex->line_length + x * (tex->bits_per_pixel / 8));
	return (*(int *)pixel);
}

/*
void	draw(int draw_top, int draw_bottom, void *col_img, int *colors, t_texture *texture, int slice_height, int top, double wallX, )
{
	int		i;

	i = draw_top;
	while (i <= draw_bottom)
	{
		double tex_yf;
		if (slice_height > 1)
			tex_yf = ((double)(i - top) / (double)(slice_height - 1)) * (double)(texture->height - 1);
		else
			tex_yf = 0.0;
		int tex_y = (int)(tex_yf + 0.5);
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= texture->height)
			tex_y = texture->height - 1;
		int tex_x = (int)(wallX * (double)texture->width);
		if (tex_x < 0)
			tex_x = 0;
		if (tex_x >= texture->width)
			tex_x = texture->width - 1;
		dst = col_addr + (i * col_line_len);
		*(unsigned int *)dst = (unsigned int)get_pixel(texture, tex_x, tex_y);
		i++;
	}
}
*/

void	draw_wall(t_vars *vars, int screen_x, int slice_height, double wallX)
{
	void	*col_img;
	int		top;
	int		bottom;
	char	*dst;

	if (!vars->texture || !vars->texture->addr)
		return ;
	top = (MAP_HEIGHT / 2) - (slice_height / 2);
	bottom = top + slice_height - 1;
	/* visible draw range (clip to screen) */
	int draw_top = (top < 0) ? 0 : top;
	int draw_bottom = (bottom >= MAP_HEIGHT) ? (MAP_HEIGHT - 1) : bottom;
	/* create a temporary 1px-wide image for this column */
	col_img = mlx_new_image(vars->mlx, 1, MAP_HEIGHT);
	if (col_img)
	{
		vars->texture->col_addr = mlx_get_data_addr(col_img, &vars->texture->col_bpp, &vars->texture->col_line_len, &vars->texture->col_endian);

		/* Use the full wall height (slice_height) when mapping texture coordinates
		   so the texture sampling keeps correct perspective. Only fill the
		   visible rows (draw_top..draw_bottom). */
		int i = draw_top;
		while (i <= draw_bottom)
		{
			double tex_yf;
			if (slice_height > 1)
				tex_yf = ((double)(i - top) / (double)(slice_height - 1)) * (double)(vars->texture->height - 1);
			else
				tex_yf = 0.0;
			int tex_y = (int)(tex_yf + 0.5);
			if (tex_y < 0)
				tex_y = 0;
			if (tex_y >= vars->texture->height)
				tex_y = vars->texture->height - 1;

			/* Compute horizontal texture coordinate from wallX fractional position */
			int tex_x = (int)(wallX * (double)vars->texture->width);
			if (tex_x < 0)
				tex_x = 0;
			if (tex_x >= vars->texture->width)
				tex_x = vars->texture->width - 1;
			/* write into the column image buffer */
			dst = vars->texture->col_addr + (i * vars->texture->col_line_len);
			*(unsigned int *)dst = (unsigned int)get_pixel(vars->texture, tex_x, tex_y);
			i++;
		}
		i = 0;
		while (i < draw_top)
		{
			dst = vars->texture->col_addr + (i * vars->texture->col_line_len);
			*(unsigned int *)dst = (unsigned int)vars->parsing->colors[1];
			i++;
		}
		i = draw_bottom + 1;
		while (i < MAP_HEIGHT)
		{
			dst = vars->texture->col_addr + (i * vars->texture->col_line_len);
			*(unsigned int *)dst = (unsigned int)vars->parsing->colors[0];
			i++;
		}
		/* blit the column to the window at x=screen_x */
		mlx_put_image_to_window(vars->mlx, vars->win, col_img, screen_x, 0);
		mlx_destroy_image(vars->mlx, col_img);
	}
	return;
}

void	draw_texture(t_vars *vars, int ray, int slice_height, double wallX, int tex_index)
{
	vars->texture->addr = mlx_get_data_addr(vars->texture->img[tex_index],
		&vars->texture->bits_per_pixel,
		&vars->texture->line_length,
		&vars->texture->endian);
	draw_wall(vars, ray, slice_height, wallX);
}
