/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:52:52 by kbossio           #+#    #+#             */
/*   Updated: 2025/10/31 16:27:22 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int mat_free(char **matrix)
{
    int i;

    i = 0;
    if (!matrix)
        return (0);
    while (matrix[i])
    {
        free(matrix[i]);
        i++;
    }
    free(matrix);
    return (0);
}

int dup_map(t_parsing *p, char *map_file)
{
    int fd;
    int i;

    fd = open(map_file, O_RDONLY);
    if (fd == -1)
        return (printf("Error"), -1);
    p->map = malloc(sizeof(char *) * (p->map_h + 1));
    if (!p->map)
    {
        printf("Memory allocation failed");
        close(fd);
        return (-1);
    }
    p->map[0] = get_next_line(fd);
    i = 0;
    while (p->map[i])
        p->map[++i] = get_next_line(fd);
    p->map[i] = NULL;
    close(fd);
    return (0);
}

int dup_matrix(char **map, t_parsing *p)
{
    int i;
    char **new_map;

    i = 0;
    while (map[i])
        i++;
    new_map = malloc(sizeof(char *) * (i + 1));
    if (!new_map)
        return (printf("Memory allocation failed"), -1);
    for (int k = 0; k < i; ++k)
    {
        new_map[k] = ft_strdup(map[k]);
        if (!new_map[k])
        {
            /* free what we already allocated */
            int t = 0;
            while (t < k)
                free(new_map[t++]);
            free(new_map);
            return (printf("Memory allocation failed"), -1);
        }
    }
    new_map[i] = NULL;
    /* now free old matrix and replace it */
    mat_free(p->map);
    p->map = new_map;
    return (0);
}
