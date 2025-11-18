/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 17:45:37 by lzorzit           #+#    #+#             */
/*   Updated: 2025/11/18 18:44:40 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minilibx/mlx.h"
#include "Cub3D.h"

static t_vars *g_vars = NULL;

static void cleanup(void)
{
	if (!g_vars)
		return ;
	if (g_vars->parsing)
	{
		if (g_vars->parsing->map)
			mat_free(g_vars->parsing->map);
		if (g_vars->parsing->textures)
			mat_free(g_vars->parsing->textures);
		if (g_vars->parsing->colors)
			free(g_vars->parsing->colors);
		free(g_vars->parsing);
		// mat_free(g_vars->parsing->textures);
		// int i = 0;
		// while (g_vars->texture && i < 4)
		// {
		// 	if (g_vars->texture->img[i])
		// 		mlx_destroy_image(g_vars->mlx, g_vars->texture->img[i]);
		// 	i++;
		// }
		// free(g_vars->texture);
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

int key_hook(int keycode, t_vars *vars)
{
	if (!vars)
		return (0);
	/* Update shared keys state in vars->keys */
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

int	close_window(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	mlx_destroy_display(vars->mlx);
	cleanup();
	exit(EXIT_SUCCESS);
	return (0);
}

int main(int argc, char **argv)
{
	t_vars *vars;
	t_parsing *p;

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
	/* Initialize runtime fields to safe defaults */
	vars->mlx = NULL;
	vars->win = NULL;
	vars->background = NULL;
	vars->fd = -1;
	vars->player = NULL;
	vars->texture = NULL;
	vars->keys = (t_keys){0, 0, 0, 0, 0, 0};
	vars->wall_scale = WALL_SCALE;
	vars->move_step = 20;
	vars->cell_size = CELL_SIZE;
	g_vars = vars;
	if (argc != 2)
	{
		printf("Usage: %s <map_file>\n", argv[0]);
		mat_free(p->textures);
		if (p->colors)
			free(p->colors);
		if (p->map)
			mat_free(p->map);
		free(p);
		return (1);
	}
	if (parsing(p, argv[1]) < 0)
	{
		printf("Parsing failed\n");
		if (p->map)
			mat_free(p->map);
		mat_free(p->textures);
		if (p->colors)
			free(p->colors);
		free(p);
		return (1);
	}
	vars->mlx = mlx_init();
	vars->win = mlx_new_window(vars->mlx, 1024, 512, "2D Map");
	printf("Map loaded successfully. Initializing player...\n");
	vars->map = p->map;
	vars->parsing = p;
	//redraw_scene(vars);
	init_player(vars);
	mlx_hook(vars->win, 2, 1L<<0, key_hook, vars);    /* KeyPress, KeyPressMask */
	mlx_hook(vars->win, 3, 1L<<1, key_release, vars);  /* KeyRelease, KeyReleaseMask */
	mlx_loop_hook(vars->mlx, move, vars);
	mlx_hook(vars->win, 17, 0, close_window, vars);    /* DestroyNotify */
	mlx_loop(vars->mlx);
	return (0);
}
