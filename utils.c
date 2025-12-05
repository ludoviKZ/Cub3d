/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:52:52 by kbossio           #+#    #+#             */
/*   Updated: 2025/12/02 18:14:17 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int	mat_free(char **matrix)
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
	return (0);
}

int	dup_map(t_parsing *p, char *map_file)
{
	int	fd;
	int	i;

	fd = open(map_file, O_RDONLY);
	if (fd == -1)
		return (printf("Error"), -1);
	p->map = malloc(sizeof(char *) * (p->map_h + 1));
	if (!p->map)
	{
		printf("Memory allocation failed");
		close(fd);
		return (-1);
	}
	p->map[0] = get_next_line(fd);
	i = 0;
	while (p->map[i])
		p->map[++i] = get_next_line(fd);
	p->map[i] = NULL;
	close(fd);
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
	return (new_map[i] = NULL, mat_free(p->map), new_map);
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
