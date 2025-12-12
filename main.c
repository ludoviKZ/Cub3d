/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 17:45:37 by lzorzit           #+#    #+#             */
/*   Updated: 2025/12/12 17:28:32 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

void	cleanup(t_vars *g_vars)
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
			mf(g_vars->parsing->map, NULL);
		if (g_vars->parsing->textures)
			mf(g_vars->parsing->textures, NULL);
		if (g_vars->parsing->colors)
			free(g_vars->parsing->colors);
		free(g_vars->parsing);
	}
	free(g_vars);
	g_vars = NULL;
}

t_parsing	*parsing_init(void)
{
	t_parsing	*p;
	int			i;

	p = malloc(sizeof(t_parsing));
	if (!p)
		return (NULL);
	p->map = NULL;
	p->map_h = 0;
	p->pl_x = -1;
	p->pl_y = -1;
	p->pl_dir = '\0';
	p->textures = malloc(sizeof(char *) * 7);
	if (!p->textures)
		return (printf("Error\nMalloc failed\n"), free(p), NULL);
	i = 0;
	while (i < 7)
		p->textures[i++] = NULL;
	p->colors = malloc(sizeof(int) * 2);
	if (!p->colors)
		return (mf(p->textures, NULL), printf("Error\nMalloc failed\n"), NULL);
	p->colors[0] = -1;
	p->colors[1] = -1;
	return (p);
}

t_vars	*init_vars(t_parsing *p)
{
	t_vars	*vars;

	vars = malloc(sizeof(t_vars));
	if (!vars)
		return (free(p->colors), mf(p->textures, NULL),
			free(p), NULL);
	vars->mlx = NULL;
	vars->win = NULL;
	vars->map = NULL;
	vars->background = NULL;
	vars->fd = -1;
	vars->parsing = NULL;
	vars->player = NULL;
	vars->texture = NULL;
	vars->keys.w = 0;
	vars->keys.a = 0;
	vars->keys.s = 0;
	vars->keys.d = 0;
	vars->keys.left = 0;
	vars->keys.right = 0;
	vars->wall_scale = WALL_SCALE;
	vars->move_step = 10;
	vars->cell_size = CELL_SIZE;
	return (vars);
}

int	main(int argc, char **argv)
{
	t_vars		*vars;
	t_parsing	*p;

	if (argc != 2)
		return (printf("Usage: %s <map_file>\n", argv[0]), 1);
	p = parsing_init();
	if (!p)
		return (1);
	vars = init_vars(p);
	if (!vars)
		return (printf("Error\nMemory allocation failed\n"), 1);
	if (parsing(p, argv[1]) < 0)
	{
		if (p->map)
			mf(p->map, NULL);
		if (p->colors)
			free(p->colors);
		printf("Error\nParsing failed\n");
		return (mf(p->textures, NULL), free(vars), free(p), 1);
	}
	vars->mlx = mlx_init();
	vars->win = mlx_new_window(vars->mlx, 1024, 512, "2D Map");
	vars->map = p->map;
	vars->parsing = p;
	return (init_player(vars), 0);
}
