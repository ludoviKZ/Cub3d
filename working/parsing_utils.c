/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 14:28:10 by kbossio           #+#    #+#             */
/*   Updated: 2026/01/18 16:49:26 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int	line_count(char *map_file)
{
	int		count;
	int		fd;
	char	*line;

	count = 0;
	fd = open(map_file, O_RDONLY);
	if (fd == -1)
		return (-1);
	line = get_next_line(fd);
	while (line != NULL)
	{
		free(line);
		count++;
		line = get_next_line(fd);
	}
	close(fd);
	return (count);
}

int	check_fl(char *line)
{
	int	j;

	j = 0;
	while (line[j] != '\n' && line[j] != '\0')
	{
		if (line[j] != '1' && line[j] != ' ')
			return (-1);
		j++;
	}
	return (0);
}

void	free_init_player(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	mlx_destroy_display(vars->mlx);
	free(vars->mlx);
	cleanup(vars);
}

int	check_player(t_parsing *p, char **map, int i, int j)
{
	static int	cont = 0;

	if (map[i][j] == 'N' || map[i][j] == 'S'
		|| map[i][j] == 'E' || map[i][j] == 'W')
	{
		p->pl_x = j;
		p->pl_y = i;
		p->pl_dir = map[i][j];
		cont++;
		if (map[i][j + 1] == '\n' || map[i][j + 1] == '\0'
			|| map[i][j - 1] == '\n' || map[i][j - 1] == '\0'
			|| map[i + 1] == NULL || map[i - 1] == NULL
			|| ft_strlen(map[i + 1]) - 1 <= j
			|| ft_strlen(map[i - 1]) - 1 <= j)
			return (2);
		return (cont);
	}
	return (cont);
}

int	flood_fill(char **map, int x, int y)
{
	int	len;

	if (y < 0 || !map[y] || x < 0)
		return (-1);
	len = ft_strlen(map[y]);
	if (map[y][len - 1] == '\n')
		len--;
	if (x >= len)
		return (-1);
	if (map[y][x] == '1' || map[y][x] == 'F')
		return (0);
	if (map[y][x] == ' ')
		return (-1);
	if (map[y][x] != 'N' && map[y][x] != 'S'
		&& map[y][x] != 'E' && map[y][x] != 'W' && map[y][x] != '0')
		return (-1);
	map[y][x] = 'F';
	if (flood_fill(map, x + 1, y) == -1 || flood_fill(map, x - 1, y) == -1
		|| flood_fill(map, x, y + 1) == -1 || flood_fill(map, x, y - 1) == -1)
		return (-1);
	return (0);
}
