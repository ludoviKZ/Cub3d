/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 14:21:52 by kbossio           #+#    #+#             */
/*   Updated: 2026/01/18 16:48:23 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

void	set_texture(t_parsing *p, int i, char *l, int *cont)
{
	char	*tmp;

	tmp = ft_strtrim(l, " \t\n");
	if (!tmp || !tmp[0])
	{
		if (tmp)
			free(tmp);
		return ;
	}
	if (p->textures[i])
		free(p->textures[i]);
	else
		cont[0]++;
	p->textures[i] = tmp;
}

int	has_xpm_ext(const char *s)
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
	int	last_valid;

	i = 0;
	last_valid = -1;
	while (map[i])
	{
		j = 0;
		skip_spaces(map[i], &j);
		if (map[i][j] != '\n' && map[i][j] != '\0')
			last_valid = i;
		i++;
	}
	if (last_valid == -1)
		return (0);
	i = -1;
	while (++i < last_valid + 1)
	{
		j = 0;
		skip_spaces(map[i], &j);
		if (map[i][j] == '\n' || map[i][j] == '\0')
			return (-1);
	}
	return (last_valid + 1);
}
