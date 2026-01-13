/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 17:28:12 by kbossio           #+#    #+#             */
/*   Updated: 2026/01/13 14:11:57 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int	check_char(char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = -1;
		while (map[i][++j] && map[i][j] != '\n')
			if (map[i][j] != '1' && map[i][j] != '0' && map[i][j] != 'N'
				&& map[i][j] != 'S' && map[i][j] != 'E'
				&& map[i][j] != 'W' && map[i][j] != ' ')
				return (-1);
		i++;
	}
	return (0);
}

static int	check_borders_and_player(t_parsing *p, char **tmp, int i, int j)
{
	int	len;

	len = ft_strlen(tmp[i]);
	if (tmp[i][len - 1] == '\n')
		len--;
	if (i == 0 || tmp[i + 1] == NULL || j == 0 || j == len - 1)
	{
		if (tmp[i][j] != '1' && tmp[i][j] != ' ')
			return (-1);
	}
	if (check_player(p, tmp, i, j) > 1)
		return (-1);
	return (0);
}

static int	validate_map_content(t_parsing *p, char **tmp)
{
	int	i;
	int	j;

	i = 0;
	while (tmp[i])
	{
		j = 0;
		while (tmp[i][j] != '\n' && tmp[i][j] != '\0')
		{
			if (check_borders_and_player(p, tmp, i, j) == -1)
				return (-1);
			j++;
		}
		i++;
	}
	return (0);
}

int	check_map(t_parsing *p, char **map)
{
	char	**tmp;

	p->pl_x = -1;
	tmp = dup_matrix(map);
	if (!tmp || check_char(tmp) == -1 || validate_map_content(p, tmp) == -1
		|| p->pl_x == -1 || flood_fill(tmp, p->pl_x, p->pl_y) == -1)
		return (mf(tmp, NULL), -1);
	return (mf(tmp, NULL), 0);
}

int	parsing(t_parsing *parsing, char *map_file)
{
	int	fd;
	int	i;

	parsing->map_h = line_count(map_file);
	if (parsing->map_h == -1)
		return (-1);
	fd = open(map_file, O_RDONLY);
	if (fd == -1)
		return (-1);
	parsing->map = malloc(sizeof(char *) * (parsing->map_h + 1));
	if (!parsing->map)
		return (close(fd), -1);
	i = 0;
	parsing->map[i] = get_next_line(fd);
	while (parsing->map[i])
		parsing->map[++i] = get_next_line(fd);
	parsing->map[i] = NULL;
	close(fd);
	if (check_textures(parsing) == -1 || check_map(parsing, parsing->map) == -1)
		return (-1);
	return (0);
}
