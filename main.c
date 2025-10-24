/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 17:39:13 by kbossio           #+#    #+#             */
/*   Updated: 2025/10/24 18:54:39 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3d.h"

int key_hook(int keycode, t_data *data)
{
    if (keycode == 65307)
	{
        mlx_destroy_window(data->mlx_ptr, data->win_ptr);
        mlx_destroy_display(data->mlx_ptr);
		mat_free(data->parsing->map);
        free(data->parsing);
        free(data);
        free(data->mlx_ptr);
        exit(EXIT_SUCCESS);
	}
    if (keycode == 119)
    {
        printf("W key pressed\n");
    }
    else if (keycode == 115)
    {
        printf("S key pressed\n");
    }
    else if (keycode == 97)
    {
        printf("A key pressed\n");
    }
    else if (keycode == 100)
    {
        printf("D key pressed\n");
    }
    else if (keycode == 65361)
    {
        printf("Left arrow key pressed\n");
    }
    else if (keycode == 65363)
    {
        printf("Right arrow key pressed\n");
    }

    return (0);
}

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
    t_data *data;
    t_parsing *p;

    p = malloc(sizeof(t_parsing));
    if (!p)
    {
        perror("Memory allocation failed");
        return (1);
    }
    parsing_init(p);
    data = malloc(sizeof(t_data));
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
    mlx_key_hook(data->win_ptr, key_hook, data);
    mlx_loop(data->mlx_ptr);
    return (0);
}