/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 16:34:03 by lzorzit           #+#    #+#             */
/*   Updated: 2025/12/05 16:56:21 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int	close_window(t_vars *vars)
{
	if (vars->texture->img[0])
	{
		mlx_destroy_image(vars->mlx, vars->texture->img[0]);
		mlx_destroy_image(vars->mlx, vars->texture->img[1]);
		mlx_destroy_image(vars->mlx, vars->texture->img[2]);
		mlx_destroy_image(vars->mlx, vars->texture->img[3]);
	}
	mlx_destroy_window(vars->mlx, vars->win);
	mlx_destroy_display(vars->mlx);
	free(vars->mlx);
	cleanup(vars);
	exit(EXIT_SUCCESS);
	return (0);
}

int key_hook(int keycode, t_vars *vars)
{
	if (!vars)
		return (0);
	if (keycode == 65307) /* Escape key */
		close_window(vars);
	if (keycode == 'w')
		vars->keys.w = 1;
	else if (keycode == 'a')
		vars->keys.a = 1;
	else if (keycode == 's')
		vars->keys.s = 1;
	else if (keycode == 'd')
		vars->keys.d = 1;
	else if (keycode == 65361) /* Left rotation (Left arrow) */
		vars->keys.left = 1;
	else if (keycode == 65363) /* Right rotation (Right arrow) */
		vars->keys.right = 1;
	return (0);
}

int key_release(int keycode, t_vars *vars)
{
	if (!vars)
		return (0);
	/* Clear keys state in vars->keys */
	if (keycode == 'w')
		vars->keys.w = 0;
	else if (keycode == 'a')
		vars->keys.a = 0;
	else if (keycode == 's')
		vars->keys.s = 0;
	else if (keycode == 'd')
		vars->keys.d = 0;
	else if (keycode == 65361) /* Left rotation (Left arrow) */
		vars->keys.left = 0;
	else if (keycode == 65363) /* Right rotation (Right arrow) */
		vars->keys.right = 0;
	return (0);
}