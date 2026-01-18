/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:52:52 by kbossio           #+#    #+#             */
/*   Updated: 2026/01/18 16:49:26 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int	mf(char **matrix, char *unused)
{
	int	i;

	if (!matrix)
		return (unused && (free(unused), 0), 0);
	i = -1;
	while (matrix[++i])
		free(matrix[i]);
	return (free(matrix), unused && (free(unused), 0), 0);
}

void	free_textures(char **textures)
{
	int	i;

	if (!textures)
		return ;
	i = 0;
	while (i < 7)
	{
		if (textures[i])
			free(textures[i]);
		i++;
	}
	free(textures);
}

void	free_dup_matrix(char **new_map, int count)
{
	int	j;

	j = 0;
	while (j < count)
		free(new_map[j++]);
	free(new_map);
}

char	**dup_matrix(char **map)
{
	int		i;
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
			return (free_dup_matrix(new_map, i),
				printf("Memory allocation failed"), NULL);
		i++;
	}
	new_map[i] = NULL;
	return (new_map);
}

int	syntax_check_rgb(t_parsing *parsing, char **textures)
{
	int		i;
	char	**parts;
	char	*trimmed;

	i = 4;
	while (i < 6)
	{
		if (!textures[i])
			return (-1);
		trimmed = ft_strtrim(textures[i], " \t\n");
		if (!trimmed)
			return (-1);
		parts = ft_split(trimmed, ',');
		if (!parts)
			return (free(trimmed), -1);
		free(trimmed);
		if (!parts[0] || !parts[1] || !parts[2] || parts[3])
			return (mf(parts, NULL), -1);
		parsing->colors[i - 4] = rgb_to_color(parts);
		if (parsing->colors[i - 4] == -1)
			return (mf(parts, NULL), -1);
		mf(parts, NULL);
		i++;
	}
	return (0);
}
