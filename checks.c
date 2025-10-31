/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 18:13:33 by kbossio           #+#    #+#             */
/*   Updated: 2025/10/31 16:48:05 by kbossio          ###   ########.fr       */
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
        if (tex_names[i][j] == '\0')
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
    dup_matrix(&p->map[i], p);
    i = 0;
    while (i < 6)
    {
        if (!p->textures[i])
            return (-1);
        printf("texture: %s\n", p->textures[i]);
        i++;
    }
    p->textures[i] = NULL;
    return (0);
}
