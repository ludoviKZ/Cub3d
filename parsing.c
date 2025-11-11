/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 17:28:12 by kbossio           #+#    #+#             */
/*   Updated: 2025/11/11 17:18:48 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int check_player(t_parsing *p, char **map, int i, int j)
{
    static int cont = 0;
    if (map[i][j] == 'N' || map[i][j] == 'S' || map[i][j] == 'E' || map[i][j] == 'W')
    {
        p->pl_x = j;
        p->pl_y = i;
        p->pl_dir = map[i][j];
        cont++;
        return (cont);
    }
    return (cont);
}

int check_fl(char *line)
{
    int j;

    j = 0;
    while (line[j] != '\n' && line[j] != '\0')
    {
        if (line[j] != '1' && line[j] != ' ')
            return (-1);
        j++;
    }
    return (0);
}

int flood_fill(char **map, int x, int y)
{
    if (map[y] == NULL || x < 0 || y < 0 || map[y][x] == '1' || map[y][x] == 'F' ||  map[y][x] == '\n')
        return (0);
    if (map[y][x] == ' ')
        map[y][x] = 'F';
    else
        return (-1);
    if (flood_fill(map, x + 1, y) == -1)
        return (-1);
    if (flood_fill(map, x - 1, y) == -1)
        return (-1);
    if (flood_fill(map, x, y + 1) == -1)
        return (-1);
    if (flood_fill(map, x, y - 1) == -1)
        return (-1);
    return (0);
}

int check_char(char **map)
{
    int i;
    int j;

    i = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j] != '\n' && map[i][j] != '\0')
        {
            if (map[i][j] != '1' && map[i][j] != '0' && map[i][j] != 'N' &&
                map[i][j] != 'S' && map[i][j] != 'E' && map[i][j] != 'W' && map[i][j] != ' ')
                return (-1);
            j++;
        }
        i++;
    }
    return (0);
}

/*the map check has to check if:
if the line above is greater it has to check when the last 0 is compared to the last 1
if there is a space it has to be surrounded by 1 or space (flood fill algorithm)
*/
int check_map(t_parsing *p, char **map)
{
    int i;
    int j;

    i = 0;
    p->pl_x = -1;
    if (check_char(map) == -1)
        return (-1);
    while (map[i])
    {
        j = 0;
        while (map[i][j] != '\n' && map[i][j] != '\0')
        {
            if (i > 0 &&(ft_strlen(map[i - 1]) - 1 <= j && map[i][j] == '0'))
                return (-1);
            if (map[i][j] == ' ' && flood_fill(map, j, i) == -1)
                return (-1);
            if (check_player(p, map, i, j) > 1)
                return (-1);
            j++;
        }
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
    char *line;
    while ((line = get_next_line(fd)) != NULL)
    {
        free(line);
        count++;
    }
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
        return (printf("Error"), -1);
    parsing->map = malloc(sizeof(char *) * (parsing->map_h + 1));
    if (!parsing->map)
    {
        printf("Error: Memory allocation failed\n");
        close(fd);
        return (-1);
    }
    parsing->map[0] = get_next_line(fd);
    i = 0;
    while (parsing->map[i])
        parsing->map[++i] = get_next_line(fd);
    parsing->map[i] = NULL;
    close(fd);
    if (check_textures(parsing) == -1)
        return (printf("Error\n"), -1);
    if (check_map(parsing, parsing->map) == -1)
        return (printf("Error\n"), -1);
    return (0);
}
