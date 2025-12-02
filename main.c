/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 17:45:37 by lzorzit           #+#    #+#             */
/*   Updated: 2025/12/02 18:12:55 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

void cleanup(t_vars *g_vars)
{
	if (!g_vars)
		return ;
	if (g_vars->player)
		free(g_vars->player);
	if (g_vars->texture)
		free(g_vars->texture);
	if (g_vars->parsing)
	{
		if (g_vars->parsing->map)
			mat_free(g_vars->parsing->map);
		if (g_vars->parsing->textures)
			mat_free(g_vars->parsing->textures);
		if (g_vars->parsing->colors)
			free(g_vars->parsing->colors);
		free(g_vars->parsing);
	}
	free(g_vars);
	g_vars = NULL;
}

int parsing_init(t_parsing *parsing)
{
	int i;

	parsing->map = NULL;
	parsing->map_h = 0;
	parsing->pl_x = -1;
	parsing->pl_y = -1;
	parsing->pl_dir = '\0';
	parsing->textures = malloc(sizeof(char *) * 7);
	if (!parsing->textures)
	{
		printf("Error\n");
		return (-1);
	}
	/* initialize texture pointers to NULL to avoid reading uninitialized memory */
	i = 0;
	while (i < 7)
		parsing->textures[i++] = NULL;
	parsing->colors = malloc(sizeof(int) * 2);
	if (!parsing->colors)
	{
		mat_free(parsing->textures);
		printf("Error\n");
		return (-1);
	}
	parsing->colors[0] = -1;
	parsing->colors[1] = -1;
	return (0);
}

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
	else if (keycode == 'i') /* Left rotation */
		vars->keys.left = 1;
	else if (keycode == 'o') /* Right rotation */
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
	else if (keycode == 'i') /* Left rotation */
		vars->keys.left = 0;
	else if (keycode == 'o') /* Right rotation */
		vars->keys.right = 0;
	return (0);
}

int main(int argc, char **argv)
{
	t_vars *vars;
	t_parsing *p;

	if (argc != 2)
		return (printf("Usage: %s <map_file>\n", argv[0]), 1);
   	p = malloc(sizeof(t_parsing));
	if (!p)
	{
		printf("Memory allocation failed");
		return (1);
	}
	parsing_init(p);
	vars = malloc(sizeof(t_vars));
	if (!vars)
	{
		mat_free(p->textures);
		if (p->colors)
			free(p->colors);
		free(p);
		printf("Memory allocation failed");
		return (1);
	}
	vars->parsing = p;
	vars->mlx = NULL;
	vars->win = NULL;
	vars->background = NULL;
	vars->fd = -1;
	vars->player = NULL;
	vars->texture = NULL;
	vars->keys = (t_keys){0, 0, 0, 0, 0, 0};
	vars->wall_scale = WALL_SCALE;
	vars->move_step = 10;
	vars->cell_size = CELL_SIZE;
	if (parsing(p, argv[1]) < 0)
	{
		printf("Parsing failed\n");
		if (p->map)
			mat_free(p->map);
		mat_free(p->textures);
		if (p->colors)
			free(p->colors);
		free(vars);
		free(p);
		return (1);
	}
	vars->mlx = mlx_init();
	vars->win = mlx_new_window(vars->mlx, 1024, 512, "2D Map");
	printf("Map loaded successfully. Initializing player...\n");
	vars->map = p->map;
	vars->parsing = p;
	init_player(vars);
	mlx_hook(vars->win, 2, 1L<<0, key_hook, vars);    /* KeyPress, KeyPressMask */
	mlx_hook(vars->win, 3, 1L<<1, key_release, vars);  /* KeyRelease, KeyReleaseMask */
	mlx_loop_hook(vars->mlx, move, vars);
	mlx_hook(vars->win, 17, 0, close_window, vars);    /* DestroyNotify */
	mlx_loop(vars->mlx);
	return (0);
}
