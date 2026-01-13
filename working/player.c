/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 16:26:51 by lzorzit           #+#    #+#             */
/*   Updated: 2026/01/06 17:00:22 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

static void	destroy_partial_textures(t_vars *vars, t_texture *t, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (t->img[i])
			mlx_destroy_image(vars->mlx, t->img[i]);
		i++;
	}
}

int	init_texture(t_vars *vars, t_texture *t, t_parsing *p)
{
	t->width[0] = 64;
	t->height[0] = 64;
	t->img[0] = mlx_xpm_file_to_image(vars->mlx, p->textures[0],
			&t->width[0], &t->height[0]);
	if (!t->img[0])
		return (-1);
	t->width[1] = 64;
	t->height[1] = 64;
	t->img[1] = mlx_xpm_file_to_image(vars->mlx, p->textures[1],
			&t->width[1], &t->height[1]);
	if (!t->img[1])
		return (destroy_partial_textures(vars, t, 1), -1);
	t->width[2] = 64;
	t->height[2] = 64;
	t->img[2] = mlx_xpm_file_to_image(vars->mlx, p->textures[2],
			&t->width[2], &t->height[2]);
	if (!t->img[2])
		return (destroy_partial_textures(vars, t, 2), -1);
	t->width[3] = 64;
	t->height[3] = 64;
	t->img[3] = mlx_xpm_file_to_image(vars->mlx, p->textures[3],
			&t->width[3], &t->height[3]);
	if (!t->img[3])
		return (destroy_partial_textures(vars, t, 3), -1);
	return (init_texture_values(t));
}

int	init_player(t_vars *vars)
{
	int	y;
	int	x;

	vars->cell_size = CELL_SIZE;
	y = vars->parsing->pl_y * vars->cell_size;
	x = vars->parsing->pl_x * vars->cell_size;
	vars->player = (t_player *)malloc(sizeof(t_player));
	if (!vars->player)
		return (-1);
	vars->texture = malloc(sizeof(t_texture));
	if (!vars->texture)
		return (free(vars->player), -1);
	if (init_texture(vars, vars->texture, vars->parsing) == -1)
		return (free_init_player(vars), printf("Error\nInvalid texture\n"), -1);
	vars->player->x = x;
	vars->player->y = y;
	set_player_dir(vars, vars->parsing->pl_dir);
	vars->wall_scale = WALL_SCALE;
	draw_field_of_view(vars);
	mlx_hook(vars->win, 2, 1L << 0, key_hook, vars);
	mlx_hook(vars->win, 3, 1L << 1, key_release, vars);
	mlx_loop_hook(vars->mlx, move, vars);
	mlx_hook(vars->win, 17, 0, close_window, vars);
	mlx_loop(vars->mlx);
	return (0);
}

void	set_player_dir(t_vars *vars, char dir)
{
	if (dir == 'N')
		vars->player->dir = 270;
	else if (dir == 'S')
		vars->player->dir = 90;
	else if (dir == 'E')
		vars->player->dir = 180;
	else if (dir == 'W')
		vars->player->dir = 0;
	else
		vars->player->dir = 0;
}

int	init_texture_values(t_texture *texture)
{
	texture->dst = NULL;
	texture->top = 0;
	texture->bottom = 0;
	texture->draw_top = 0;
	texture->draw_bottom = 0;
	texture->tex_index = 0;
	return (0);
}
