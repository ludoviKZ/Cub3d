/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 17:51:06 by lzorzit           #+#    #+#             */
/*   Updated: 2025/12/02 17:03:58 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

void rotate(t_vars *vars, int angle)                                                                              
{
	if (!vars->player)
		return;

	vars->player->dir = (vars->player->dir + angle);
	if (vars->player->dir >= 360)
		vars->player->dir -= 360;
	else if (vars->player->dir < 0)
		vars->player->dir += 360;
	printf("Player direction: %d degrees\n", vars->player->dir);
}

int validate_move(t_vars *vars, double *ver, double *side)
{
	t_moveval move;

	if (!vars || !vars->player)
		return (-1);
	move.new_x = vars->player->x + *side;
	move.new_y = vars->player->y + *ver;
	if (!vars->map)
		return (-1);
	move.rows = 0;
	while (vars->map[move.rows])
		move.rows++;
	move.cell_row = (int)(move.new_y / vars->cell_size);
	if (move.cell_row < 0 || move.cell_row >= move.rows)
		return (-1);
	move.cell_col = (int)(move.new_x / vars->cell_size);
	if (move.cell_col < 0 || move.cell_col >= ft_strlen(vars->map[move.cell_row]))
		return (-1);
	if (vars->map[move.cell_row][move.cell_col] == '1')
		return (-1);
	if (vars->map[move.cell_row][move.cell_col] == 'F')
		return (-1);
	if (check_for_close_walls(vars, move) == -1)
		return (-1);
	return (0);
}
int check_for_close_walls(t_vars *vars, t_moveval move)
{
	move.cell_row2 = (int)((move.new_y + 3) / vars->cell_size);
	move.cell_col2 = (int)((move.new_x + 3) / vars->cell_size);
	move.cell_row3 = (int)((move.new_y + -3) / vars->cell_size);
	move.cell_col3 = (int)((move.new_x + -3) / vars->cell_size);
	if (move.cell_row2 < 0 || move.cell_row2 >= move.rows || move.cell_col2 < 0 || move.cell_col2 >= ft_strlen(vars->map[move.cell_row2]))
		return (-1);
	if (vars->map[move.cell_row2][move.cell_col2] == '1' || vars->map[move.cell_row][move.cell_col2] == '1' 
		|| vars->map[move.cell_row2][move.cell_col] == '1')
		return (-1);
	if (vars->map[move.cell_row2][move.cell_col2] == 'F' || vars->map[move.cell_row][move.cell_col2] == 'F'
		 || vars->map[move.cell_row2][move.cell_col] == 'F')
		return (-1);
	if (vars->map[move.cell_row3][move.cell_col3] == '1' || vars->map[move.cell_row][move.cell_col3] == '1' 
		|| vars->map[move.cell_row3][move.cell_col] == '1')
		return (-1);
	if (vars->map[move.cell_row3][move.cell_col3] == 'F' || vars->map[move.cell_row][move.cell_col3] == 'F' 
		|| vars->map[move.cell_row3][move.cell_col] == 'F')
		return (-1);
	if (vars->map[move.cell_row2][move.cell_col3] == '1' || vars->map[move.cell_row3][move.cell_col2] == '1')
		return (-1);
	if (vars->map[move.cell_row2][move.cell_col3] == 'F' || vars->map[move.cell_row3][move.cell_col2] == 'F')
		return (-1);
	return (0);
}

/* Loop hook: poll keys stored in vars->keys and move player accordingly. */
int move(void *param)
{
	t_vars *vars = (t_vars *)param;
	int ver = 0;
	int side = 0;

	if (!vars || !vars->player)
		return (0);
	if (vars->keys.w)
		ver -= vars->move_step;
	if (vars->keys.s)
		ver += vars->move_step;
	if (vars->keys.a)
		side -= vars->move_step;
	if (vars->keys.d)
		side += vars->move_step;
	if (side != 0 && ver != 0)
	{
		ver = ver / sqrt(2.0);
		side = side / sqrt(2.0);
	}
	if ( (!vars->keys.left || !vars->keys.right) && (vars->keys.left || vars->keys.right) )
	{
		if (vars->keys.left)
			rotate(vars, -3);
		if (vars->keys.right)
			rotate(vars, 3);
		if (ver == 0 && side == 0)
			draw_field_of_view(vars);
		if (ver == 0 && side == 0)
			return (0);
	}
	double rad = (vars->player->dir * M_PI) / 180.0;
	double dx_f = -cos(rad) * (double)ver;
	double dy_f = -sin(rad) * (double)ver;
	double dx_s = -sin(rad) * (double)side;
	double dy_s =  cos(rad) * (double)side;
	double delta_x = dx_f + dx_s;
	double delta_y = dy_f + dy_s;

	if (validate_move(vars, &delta_y, &delta_x) == 0)
	{
		vars->player->y += delta_y;
		vars->player->x += delta_x;
	}
	if (ver != 0 || side != 0)
		draw_field_of_view(vars);
	return (0);
}
int dir_e_move(t_vars *vars, int ver, int side)
{
	t_movecalc move;
	if ( (!vars->keys.left || !vars->keys.right) && (vars->keys.left || vars->keys.right) )
	{
		if (vars->keys.left)
			rotate(vars, -3);
		if (vars->keys.right)
			rotate(vars, 3);
		if (ver == 0 && side == 0)
			draw_field_of_view(vars);
		if (ver == 0 && side == 0)
			return (0);
	}
	move.rad = (vars->player->dir * M_PI) / 180.0;
	move.delta_x = -cos(move.rad) * (double)ver + -sin(move.rad) * (double)side;
	// move.delta_y = -sin(move.rad) * (double)ver; + cos(move.rad) * (double)side;

	if (validate_move(vars, &move.delta_y, &move.delta_x) == 0)
	{
		vars->player->y += move.delta_y;
		vars->player->x += move.delta_x;
	}
	if (ver != 0 || side != 0)
		draw_field_of_view(vars);
	return (0);
}
