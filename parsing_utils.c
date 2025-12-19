/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 14:28:10 by kbossio           #+#    #+#             */
/*   Updated: 2025/12/19 18:04:50 by kbossio          ###   ########.fr       */
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
