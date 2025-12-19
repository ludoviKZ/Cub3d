/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 14:21:52 by kbossio           #+#    #+#             */
/*   Updated: 2025/12/19 14:37:47 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

static int	has_xpm_ext(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = ft_strlen(s);
	if (len < 4)
		return (0);
	return (ft_strncmp(s + len - 4, ".xpm", 4) == 0);
}

int	skip_spaces(char *line, int *index)
{
	while (line[*index] && (line[*index] == ' ' || line[*index] == '\t'))
		(*index)++;
	return (0);
}

int	check_matr(char **str)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (str[i][j] != '\n' && str[i][j] != '\0')
		{
			j++;
		}
		if (j == 0)
			return (-1);
		i++;
	}
	return (0);
}

int	check_lines(char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		skip_spaces(map[i], &j);
		if (map[i][j] == '\n' || map[i][j] == '\0')
			return (i);
		i++;
	}
	return (i);
}
