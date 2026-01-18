/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 18:13:33 by kbossio           #+#    #+#             */
/*   Updated: 2026/01/18 17:35:01 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int	check_tex(char *l, int *cont, t_parsing *p)
{
	int			i;
	int			j;
	int			k;
	static char	*tn[7] = {"NO", "SO", "WE", "EA", "F", "C", NULL};

	i = -1;
	k = 0;
	skip_spaces(l, &k);
	if (l[k] == '\0' || l[k] == '\n')
		return (0);
	while (++i < 6)
	{
		j = 0;
		while (tn[i][j] != '\0' && l[j + k] == tn[i][j])
			j++;
		if (tn[i][j] == '\0' && (l[j + k] == ' ' || l[j + k] == '\t'))
			return (set_texture(p, i, l + j + k, cont));
	}
	return (-1);
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

	i = -1;
	while (++i < 6)
		if (!textures[i] || (i < 4 && syntax_check_helper(textures, i) == -1))
			return (-1);
	return (syntax_check_rgb(parsing, textures));
}

int	dup_and_set_map(t_parsing *p, int i)
{
	char	**new_map;

	new_map = dup_matrix(&p->map[i]);
	if (!new_map)
		return (-1);
	return (mf(p->map, NULL), p->map = new_map, 0);
}

int	check_textures(t_parsing *p)
{
	int		i;
	int		j;
	int		cont;

	i = 0;
	cont = 0;
	while (p->map[i] && cont < 6)
		if (check_tex(p->map[i++], &cont, p) == -1)
			return (-1);
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
	if (p->map_h <= 0 || dup_and_set_map(p, i) == -1)
		return (-1);
	if (syntax_check(p, p->textures) == -1)
		return (-1);
	return (p->textures[6] = NULL, 0);
}
