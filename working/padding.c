/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 16:50:00 by kbossio           #+#    #+#             */
/*   Updated: 2026/01/18 16:49:26 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int	get_max_width(char **map)
{
	int	i;
	int	len;
	int	max;

	i = 0;
	max = 0;
	while (map[i])
	{
		len = ft_strlen(map[i]);
		if (map[i][len - 1] == '\n')
			len--;
		if (len > max)
			max = len;
		i++;
	}
	return (max);
}

char	*pad_line(char *line, int max_width)
{
	char	*new_line;
	int		len;
	int		i;

	len = ft_strlen(line);
	if (line[len - 1] == '\n')
		len--;
	new_line = malloc(max_width + 2);
	if (!new_line)
		return (NULL);
	i = -1;
	while (++i < len)
		new_line[i] = line[i];
	while (i < max_width)
		new_line[i++] = ' ';
	new_line[i++] = '\n';
	new_line[i] = '\0';
	return (new_line);
}

char	**pad_map(char **map)
{
	char	**padded;
	int		max_width;
	int		i;

	max_width = get_max_width(map);
	i = 0;
	while (map[i])
		i++;
	padded = malloc(sizeof(char *) * (i + 1));
	if (!padded)
		return (NULL);
	i = -1;
	while (map[++i])
	{
		padded[i] = pad_line(map[i], max_width);
		if (!padded[i])
		{
			while (--i >= 0)
				free(padded[i]);
			return (free(padded), NULL);
		}
	}
	padded[i] = NULL;
	return (padded);
}
