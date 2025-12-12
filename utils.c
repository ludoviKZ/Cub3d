/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:52:52 by kbossio           #+#    #+#             */
/*   Updated: 2025/12/12 17:28:06 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int	mf(char **matrix, char *unused)
{
	int	i;

	i = 0;
	if (!matrix)
		return (0);
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
	if (unused)
		free(unused);
	return (0);
}

char	**dup_matrix(char **map, t_parsing *p)
{
	int		i;
	int		j;
	char	**new_map;

	i = 0;
	while (map[i])
		i++;
	new_map = malloc(sizeof(char *) * (i + 1));
	if (!new_map)
		return (printf("Memory allocation failed"), NULL);
	i = 0;
	while (map[i])
	{
		new_map[i] = ft_strdup(map[i]);
		if (!new_map[i])
		{
			j = 0;
			while (j < i)
				free(new_map[j++]);
			free(new_map);
			return (printf("Memory allocation failed"), NULL);
		}
		i++;
	}
	return (new_map[i] = NULL, mf(p->map, NULL), new_map);
}

int	isnum(char **str)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (str[i][j] != '\0')
		{
			if (str[i][j] < '0' || str[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	rgb_to_color(char **rgb)
{
	int	r;
	int	g;
	int	b;

	if ((!rgb || !rgb[0] || !rgb[1] || !rgb[2]) || !isnum(rgb))
		return (-1);
	r = ft_atoi(rgb[0]);
	g = ft_atoi(rgb[1]);
	b = ft_atoi(rgb[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (-1);
	return ((r << 16) | (g << 8) | b);
}
