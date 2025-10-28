/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 17:39:13 by kbossio           #+#    #+#             */
/*   Updated: 2025/10/28 16:02:46 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int parsing_init(t_parsing *parsing)
{
    parsing->map = NULL;
    parsing->map_h = 0;
    parsing->pl_x = -1;
    parsing->pl_y = -1;
    parsing->pl_dir = '\0';
    return (0);
}

int    main(int argc, char **argv)
{
    t_vars *data;
    t_parsing *p;

    p = malloc(sizeof(t_parsing));
    if (!p)
    {
        perror("Memory allocation failed");
        return (1);
    }
    parsing_init(p);
    data = malloc(sizeof(t_vars));
    data->parsing = p;
    if (argc != 2)
    {
        printf("Usage: %s <map_file>\n", argv[0]);
        return (1);
    }
    if (parsing(p, argv[1]) < 0)
    {
        printf("Parsing failed\n");
        return (1);
    }
    data->mlx_ptr = mlx_init();
    data->win_ptr = mlx_new_window(data->mlx_ptr, 800, 600, "Cub3D");
    printf("Window created successfully!\n");

    return (0);
}