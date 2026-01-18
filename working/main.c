/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 17:45:37 by lzorzit           #+#    #+#             */
/*   Updated: 2026/01/18 16:49:26 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int	check_extension(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	return (len >= 5 && ft_strncmp(filename + len - 4, ".cub", 4) == 0);
}

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
			free_textures(g_vars->parsing->textures);
		if (g_vars->parsing->colors)
			free(g_vars->parsing->colors);
		free(g_vars->parsing);
	}
	free(g_vars);
	g_vars = NULL;
}

int	handle_parsing_error(t_parsing *p, t_vars *vars)
{
	if (p->map)
		mf(p->map, NULL);
	if (p->colors)
		free(p->colors);
	printf("Error\nParsing failed\n");
	free_textures(p->textures);
	free(vars);
	free(p);
	return (1);
}

int	main(int argc, char **argv)
{
	t_vars		*vars;
	t_parsing	*p;

	if (argc != 2)
		return (printf("Usage: %s <map_file>\n", argv[0]), 1);
	p = parsing_init(argv);
	if (!p)
		return (1);
	vars = init_vars(p);
	if (!vars)
		return (printf("Error\nMemory allocation failed\n"), 1);
	if (parsing(p, argv[1]) < 0)
		return (handle_parsing_error(p, vars));
	vars->mlx = mlx_init();
	vars->win = mlx_new_window(vars->mlx, 1024, 512, "2D Map");
	vars->map = p->map;
	vars->parsing = p;
	print_map(vars->map);
	return (init_player(vars));
}
