/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 13:30:00 by kbossio           #+#    #+#             */
/*   Updated: 2026/01/13 14:07:37 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

t_parsing	*parsing_init(char **argv)
{
	t_parsing	*p;
	int			i;

	if (!check_extension(argv[1]))
		return (printf("Error\nInvalid file extension\n"), NULL);
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

int	isnum(char **str)
{
	int	i;
	int	j;

	i = -1;
	while (str[++i])
	{
		j = -1;
		while (str[i][++j])
		{
			if (str[i][j] < '0' || str[i][j] > '9')
				return (-1);
		}
	}
	return (0);
}

int	rgb_to_color(char **rgb)
{
	int	r;
	int	g;
	int	b;

	if (isnum(rgb) == -1)
		return (-1);
	r = ft_atoi(rgb[0]);
	g = ft_atoi(rgb[1]);
	b = ft_atoi(rgb[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (-1);
	return (r << 16 | g << 8 | b);
}

void	print_map(char **map)
{
	int	i;

	i = 0;
	while (map[i])
		printf("%s", map[i++]);
}
