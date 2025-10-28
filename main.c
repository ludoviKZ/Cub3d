/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 17:45:37 by lzorzit           #+#    #+#             */
/*   Updated: 2025/10/28 16:09:52 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minilibx/mlx.h"
#include "Cub3D.h"

int	key_hook(int keycode, t_vars *vars)
{
	if (keycode == 's' )
		move(vars, 1, 0);
	else if (keycode == 'a' )
		move(vars, 0, -1);
	else if (keycode == 'w' )
		move(vars, -1, 0);
	else if (keycode == 'd' )
		move(vars, 0, +1);
	else if (keycode == 'i' ) // Left arrow key
		rotate(vars, -5);
	else if (keycode == 'o' ) // Right arrow key
		rotate(vars, 5);
	else if (keycode == 'f') // Escape key
	{
		draw_field_of_view(vars, 0xaaaaaa); // Redraw FOV after movement
	}
	else if (keycode == 65307) // Escape key
	{
		mlx_destroy_window(vars->mlx, vars->win);
		mlx_destroy_display(vars->mlx);
		mat_free(vars->parsing->map);
		free(vars->parsing);
		free(vars);
		exit(EXIT_SUCCESS);
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_vars *vars;
    t_parsing *p;

   	p = malloc(sizeof(t_parsing));
    if (!p)
    {
        perror("Memory allocation failed");
        return (1);
    }
    parsing_init(p);
    vars = malloc(sizeof(t_vars));
    vars->parsing = p;
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
	vars->mlx = mlx_init();
	vars->win = mlx_new_window(vars->mlx, 1024, 512, "2D Map");
	draw_square2D(vars->map, vars->mlx, vars->win);
	init_player(&vars);
	mlx_key_hook(vars->win, key_hook, vars);
	mlx_loop(vars->mlx);
	return (0);
}
