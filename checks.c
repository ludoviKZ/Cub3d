/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 18:13:33 by kbossio           #+#    #+#             */
/*   Updated: 2025/11/07 18:47:51 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int skip_spaces(char *line, int *index)
{
	while (line[*index] == ' ' || line[*index] == '\t')
		(*index)++;
	return (0);
}

int check_matr(char **str)
{
	int i;
	int j;

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

int check_lines(char **map)
{
	int i;

	int j;

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

void check_tex(char *line, int *cont, t_parsing *p)
{
	int i;
	int j;
	int k;
	char *tex_names[7] = {"NO", "SO", "WE", "EA", "F", "C", NULL};
	
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
			if (!p->textures[i])
				cont[0]++;
			p->textures[i] = ft_strdup(line + k);
		}
		i++;
	}
}

int syntax_check_rgb(t_parsing *parsing, char **textures)
{
	int i;
	char **parts;
	char *trimmed;
	char *c;
	int k;
	int idx;
	long n;

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
		printf("parts[] for texture %d: %s, %s, %s\n", i, parts[0], parts[1], parts[2]);
		if (!parts[0] || !parts[1] || !parts[2] || parts[3])
			return (printf("Error: Invalid RGB values\n"), mat_free(parts), -1);
		k = 0;
		while (k < 3)
		{
			c = ft_strtrim(parts[k], " \t\n");
			if (!c)
				return (mat_free(parts), -1);
			idx = 0;
			if (c[0] == '\0')
				return (free(c), mat_free(parts), -1);
			while (c[idx])
			{
				if (c[idx] < '0' || c[idx] > '9')
					return (free(c), mat_free(parts), -1);
				idx++;
			}
			n = ft_atoi(c);
			free(c);
			if (n < 0 || n > 255)
			{
				mat_free(parts);
				return (-1);
			}
			k++;
		}
		parsing->textures[i] = ft_itoa(rgb_to_color(parts));
		printf("texture[%d] color: %s\n", i, parsing->textures[i]);
		mat_free(parts);
		i++;
	}
	return (0);
}

int syntax_check(t_parsing *parsing, char **textures)
{
	int i;
	int fd;
	char *trimmed;

	i = 0;
	while (i < 6)
	{
		if (!textures[i])
			return (-1);
		if (i < 4)
		{
			trimmed = ft_strtrim(textures[i], " \t\n");
			if (!trimmed)
				return (-1);
			fd = open(trimmed, O_RDONLY);
			// if (fd == -1)
			//     return (free(trimmed), -1);
			close(fd);
			free(trimmed);
		}
		i++;
	}
	if (syntax_check_rgb(parsing, textures) == -1)
		return (-1);
	return (0);
}

int check_textures(t_parsing *p)
{
	int i;
	int j;
	int cont;

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
	p->textures[i] = NULL;
	return (0);
}
