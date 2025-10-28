/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 18:13:33 by kbossio           #+#    #+#             */
/*   Updated: 2025/10/28 18:39:58 by kbossio          ###   ########.fr       */
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

int check_textures(t_parsing *p)
{
    int i;
    int j;

    i = 0;
    while (p->map[i])
    {
        j = 0;
        skip_spaces(p->map[i], &j);
        if (p->map[i][j] && p->map[i][j] == 'N' && p->map[i][j + 1] != 'O')
        {
            j += 2;
            skip_spaces(p->map[i], &j);
            p->textures[0] = ft_strdup(&p->map[i][j]);
        }
        else if (p->map[i][j] && p->map[i][j] == 'S' && p->map[i][j + 1] == 'O')
        {
            j += 2;
            skip_spaces(p->map[i], &j);
            p->textures[1] = ft_strdup(&p->map[i][j]);
        }
        else if (p->map[i][j] && p->map[i][j] == 'W' && p->map[i][j + 1] == 'E')
        {
            j += 2;
            skip_spaces(p->map[i], &j);
            p->textures[2] = ft_strdup(&p->map[i][j]);
        }
        else if (p->map[i][j] && p->map[i][j] == 'E' && p->map[i][j + 1] == 'A')
        {
            j += 2;
            skip_spaces(p->map[i], &j);
            p->textures[3] = ft_strdup(&p->map[i][j]);
        }
        else if (p->map[i][j] && p->map[i][j] == 'F')
        {
            j += 1;
            skip_spaces(p->map[i], &j);
            p->textures[4] = ft_strdup(&p->map[i][j]);
        }
        else if (p->map[i][j] && p->map[i][j] == 'C')
        {
            j += 1;
            skip_spaces(p->map[i], &j);
            p->textures[5] = ft_strdup(&p->map[i][j]);
        }
        i++;
    }
    return (0);
}