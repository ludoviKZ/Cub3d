/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 17:45:37 by lzorzit           #+#    #+#             */
/*   Updated: 2025/10/28 14:28:52 by lzorzit          ###   ########.fr       */
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
	return (0);
}

int main(void)
{
	t_vars vars;
	int map_size[2] = {10, 10}; // Example map size (width, height)
	char *example_map[] = {
		"1111111111",
		"1000000001",
		"1011111101",
		"1000000001",
		"1011111001",
		"1000000001",
		"1011111101",
		"1000000001",
		"1111111111",
		"1111111111",
		NULL
	};

	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, 1024, 512, "2D Map");
	/* make the map available to other code that validates moves */
	vars.map = example_map;
	draw_square2D(example_map, map_size, vars.mlx, vars.win);
	init_player(&vars);
	mlx_key_hook(vars.win, key_hook, &vars);
	mlx_loop(vars.mlx);
	return 0;
}
