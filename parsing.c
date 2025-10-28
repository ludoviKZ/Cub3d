/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 17:28:12 by kbossio           #+#    #+#             */
/*   Updated: 2025/10/28 15:48:36 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3d.h"

void check_player(t_parsing *p, char **map, int i, int j)
{
    if (map[i][j] == 'N' || map[i][j] == 'S' || map[i][j] == 'E' || map[i][j] == 'W')
    {
        p->pl_x = j;
        p->pl_y = i;
        p->pl_dir = map[i][j];
    }
}

// int check_char(char c, char *set)
// {
//     int i;

//     i = 0;
//     while (set[i])
//     {
//         if (c == set[i])
//             return (1);
//         i++;
//     }
//     return (0);
// }

// int check_surrounded(t_parsing *p, char **map, int i, int j)
// {
//     int a;
//     int b;
//     char c[] = {'1', ' ', '\n', '\0'};

//     a = 0;
//     if (map[i][j] == ' ')
//     {
        
//     }
//     return (0);
// }

int check_map(t_parsing *p, char **map)
{
    int i;
    int j;

    i = 0;
    j = 0;
    p->pl_x = -1;
    while (map[i])
    {
        while (map[i][j] != '\n' && map[i][j] != '\0')
        {
            if ((i == 0 && map[i][j] != '1') || (i == p->map_h - 1 && map[i][j] != '1') ||
                (j == 0 && map[i][j] != '1') || (j == ft_strlen(map[i]) - 1 && map[i][j] != '1'))
                return (-1);
            if (map[i][j] != '1' && map[i][j] != '0' && map[i][j] != 'N' &&
                map[i][j] != 'S' && map[i][j] != 'E' && map[i][j] != 'W')
                return (-1);
            // if (check_surrounded(p, map, i, j) == -1)
            //     return (-1);
            check_player(p, map, i, j);
            j++;
        }
        j = 0;
        i++;
    }
    if (p->pl_x == -1)
        return (-1);
    return (0);
}

int line_count(char *map_file)
{
    int count;
    int fd;

    count = 0;
    fd = open(map_file, O_RDONLY);
    if (fd == -1)
        return (-1);
    while (get_next_line(fd))
        count++;
    close(fd);
    return (count);
}

int parsing(t_parsing *parsing, char *map_file)
{
    int fd;
    int i;
    
    parsing->map_h = line_count(map_file);
    if (parsing->map_h == -1)
        return (-1);
    fd = open(map_file, O_RDONLY);
    if (fd == -1)
        return (perror("Error"), -1);
    parsing->map = malloc(sizeof(char *) * (parsing->map_h + 1));
    if (!parsing->map)
    {
        perror("Memory allocation failed");
        close(fd);
        return (-1);
    }
    parsing->map[0] = get_next_line(fd);
    i = 0;
    while (parsing->map[i])
        parsing->map[++i] = get_next_line(fd);
    parsing->map[i] = NULL;
    close(fd);
    if (check_map(parsing, parsing->map) == -1)
        return (perror("Error"), -1);
    return (0);
}
