/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 18:13:33 by kbossio           #+#    #+#             */
/*   Updated: 2025/12/19 17:56:37 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

void	check_tex(char *l, int *cont, t_parsing *p)
{
	int			i;
	int			j;
	int			k;
	static char	*tn[7] = {"NO", "SO", "WE", "EA", "F", "C", NULL};

	i = 0;
	while (i < 6)
	{
		j = 0;
		k = 0;
		skip_spaces(l, &k);
		while (tn[i][j] != '\0' && l[j + k] == tn[i][j])
			j++;
		if (tn[i][j] == '\0' && (l[j + k] == ' ' || l[j + k] == '\t'))
			set_texture(p, i, l + j + k, cont);
		i++;
	}
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

int	syntax_check_helper(char **textures, int i)
{
	int		fd;
	char	*trimmed;

	trimmed = ft_strtrim(textures[i], " \t\n");
	if (!has_xpm_ext(trimmed))
		return (free(trimmed), -1);
	fd = open(trimmed, O_RDONLY | __O_DIRECTORY);
	if (fd != -1)
		return (close(fd), free(trimmed), -1);
	fd = open(trimmed, O_RDONLY);
	if (fd == -1)
		return (free(trimmed), -1);
	close(fd);
	free(trimmed);
	return (0);
}

int	syntax_check(t_parsing *parsing, char **textures)
{
	int		i;

	i = 0;
	while (i < 6)
	{
		if (!textures[i])
			return (-1);
		if (i < 4)
		{
			if (syntax_check_helper(textures, i) == -1)
				return (-1);
		}
		i++;
	}
	if (syntax_check_rgb(parsing, textures) == -1)
		return (-1);
	return (0);
}

int	check_textures(t_parsing *p)
{
	int		i;
	int		j;
	int		cont;

	i = 0;
	cont = 0;
	while (p->map[i] && cont < 6)
		check_tex(p->map[i++], &cont, p);
	if (cont < 6)
		return (-1);
	while (p->map[i])
	{
		j = 0;
		skip_spaces(p->map[i], &j);
		if (p->map[i][j] != '\0' && p->map[i][j] != '\n')
			break ;
		i++;
	}
	p->map_h = check_lines(&p->map[i]);
	p->map = dup_matrix(&p->map[i], p);
	if (!p->map || syntax_check(p, p->textures) == -1)
		return (-1);
	p->textures[6] = NULL;
	return (0);
}
