/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 18:13:33 by kbossio           #+#    #+#             */
/*   Updated: 2025/11/25 19:16:15 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

/* Return 1 if `s` ends with ".xpm" (case-sensitive), 0 otherwise. */
static int	has_xpm_ext(const char *s)
{
	size_t len;

	if (!s)
		return (0);
	len = ft_strlen(s);
	if (len < 4)
		return (0);
	return (ft_strncmp(s + len - 4, ".xpm", 4) == 0);
}

int	skip_spaces(char *line, int *index)
{
	while (line[*index] == ' ' || line[*index] == '\t')
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

void	check_tex(char *line, int *cont, t_parsing *p)
{
	int		i;
	int		j;
	int		k;
	char	*tex_names[7] = {"NO", "SO", "WE", "EA", "F", "C", NULL};
	char	*tmp;

	i = 0;
	while (i < 6)
	{
		j = 0;
		k = 0;
		skip_spaces(line, &k);
		while (tex_names[i][j] != '\0')
		{
			if (line[j + k] != tex_names[i][j])
				break ;
			j++;
		}
		if (tex_names[i][j] == '\0' && line[j + k] == ' ')
		{
			k += j;
			skip_spaces(line, &k);
			tmp = ft_strtrim(line + k, " \t\n");
			if (!tmp)
				return ;
			if (!p->textures[i])
				cont[0]++;
			p->textures[i] = tmp;
		}
		i++;
	}
}

int	syntax_check_rgb(t_parsing *parsing, char **textures)
{
	int		i;
	char	**parts;
	char	*trimmed;
	char	*c;
	int		k;

	/* textures[4] = F (floor), textures[5] = C (ceiling) */
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
			return (printf("Error: Invalid RGB values\n"), mat_free(parts), -1);
		k = 0;
		while (k < 3)
		{
			c = ft_strtrim(parts[k], " \t\n");
			if (!c)
				return (mat_free(parts), -1);
			if (c[0] == '\0')
				return (free(c), mat_free(parts), -1);
			/* allow decimal or hex digits here; hex_conv will validate fully */
			free(c);
			k++;
		}
		/* convert to integer color and store into parsing->colors (0=floor,1=ceiling)
		   idx = i - 4 maps 4->0, 5->1 */
		parsing->colors[i - 4] = rgb_to_color(parts);
		if (parsing->colors[i - 4] == -1)
			return (printf("Error\n"), mat_free(parts), -1);
		/* free original texture string since we no longer need the textual RGB */
		free(textures[i]);
		textures[i] = NULL;
		mat_free(parts);
		i++;
	}
	return (0);
}


int	syntax_check(t_parsing *parsing, char **textures)
{
	int		i;
	int		fd;
	char	*trimmed;

	i = 0;
	while (i < 6)
	{
		if (!textures[i])
			return (-1);
		if (i < 4)
		{
			trimmed = ft_strtrim(textures[i], " \t\n");
			if (!trimmed || !has_xpm_ext(trimmed))
				return (-1);
			fd = open(trimmed, O_RDONLY);
			if (fd == -1)
				return (free(trimmed), -1);
			close(fd);
			free(trimmed);
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
	{
		check_tex(p->map[i], &cont, p);
		i++;
	}
	if (cont < 6)
		return (printf("Error: Missing textures\n"), -1);
	j = 0;
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
	i = 0;
	if (syntax_check(p, p->textures) == -1)
		return (-1);
	/* ensure the textures array is NULL-terminated (index 6) */
	p->textures[6] = NULL;
	return (0);
}
