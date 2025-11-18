#include <math.h>
#include "Cub3D.h"

int init_texture(t_vars *vars, t_texture *texture, t_parsing *parsing)
{
	int tex_w;
	int tex_h;

	tex_w = 64;
	tex_h = 64;
	texture->img[0] = mlx_xpm_file_to_image(vars->mlx, parsing->textures[0], &tex_w, &tex_h);
	if (!texture->img[0])
		return (-1);
	texture->img[1] = mlx_xpm_file_to_image(vars->mlx, parsing->textures[1], &tex_w, &tex_h);
	if (!texture->img[1])
		return (-1);
	texture->img[2] = mlx_xpm_file_to_image(vars->mlx, parsing->textures[2], &tex_w, &tex_h);
	if (!texture->img[2])
		return (-1);
	texture->img[3] = mlx_xpm_file_to_image(vars->mlx, parsing->textures[3], &tex_w, &tex_h);
	if (!texture->img[3])
		return (-1);
	texture->height = tex_h;
	texture->width = tex_w;
	return (0);
}

int	init_player(t_vars *vars)
{
	int	y;
	int	x;

	vars->cell_size = CELL_SIZE;
	y = vars->parsing->pl_y * vars->cell_size;
	x = vars->parsing->pl_x * vars->cell_size;
	vars->player = (t_player *)malloc(sizeof(t_player));
	if (!vars->player)
		return (-1);
	vars->texture = malloc(sizeof(t_texture));
	if (!vars->texture)
		return (free(vars->player), -1);
	if (init_texture(vars, vars->texture, vars->parsing) == -1)
		return (free(vars->player), free(vars->texture), -1);
	vars->player->x = x;
	vars->player->y = y;
	/* Initialize player direction from parsing->pl_dir if available. Map
	   compass directions to degrees (0 = East, 90 = North). */
	if (vars->parsing && vars->parsing->pl_dir)
	{
		if (vars->parsing->pl_dir == 'N')
			vars->player->dir = 90;
		else if (vars->parsing->pl_dir == 'S')
			vars->player->dir = 270;
		else if (vars->parsing->pl_dir == 'E')
			vars->player->dir = 0;
		else if (vars->parsing->pl_dir == 'W')
			vars->player->dir = 180;
		else
			vars->player->dir = 0;
	}
	else
		vars->player->dir = 0;
	vars->wall_scale = WALL_SCALE;
	vars->move_step = 20;
	 //redraw_scene(vars);
	 /* draw full-screen ceiling and floor once into vars->background so the
		 per-column rendering can blit it for each ray. Use mid/ (mid-1) so
		 ceiling is y=0..mid-1 and floor is mid..MAP_HEIGHT-1 */
	//  {
	// 	  int mid = MAP_HEIGHT / 2;
	// 	  draw_background(vars, 0, mid, mid - 1, vars->parsing->colors[1], vars->parsing->colors[0]);
	//  }
	draw_field_of_view(vars); // draw 3D projection across the screen
	return (0);
}
void rotate(t_vars *vars, int angle)
{
	if (!vars->player)
		return;
	/* Redraw background, update direction and re-render 3D view */
	// redraw_scene(vars);
	vars->player->dir = (vars->player->dir + angle);
	if (vars->player->dir >= 360)
		vars->player->dir -= 360;
	else if (vars->player->dir < 0)
		vars->player->dir += 360;
	draw_field_of_view(vars);
	printf("Player direction: %d degrees\n", vars->player->dir);
}
int validate_move(t_vars *vars, double ver, double side)
{
	double new_x;
	double new_y;

	if (!vars || !vars->player)
		return (-1);
	new_x = vars->player->x + side;
	new_y = vars->player->y + ver;
	if (new_x < 0 || new_x >= MAP_WIDTH || new_y < 0 || new_y >= MAP_HEIGHT)
		return (-1);
	if ((new_x + 3) < 0 || (new_x + 3) >= MAP_WIDTH || (new_y + 3) < 0 || (new_y + 3) >= MAP_HEIGHT)
		return (-1);
	if (!vars->map)
		return (-1);
	int rows = 0;
	while (vars->map[rows])
		rows++;
	int cell_row = (int)(new_y / vars->cell_size);
	if (cell_row < 0 || cell_row >= rows)
		return (-1);
	int cols = ft_strlen(vars->map[cell_row]);
	int cell_col = (int)(new_x / vars->cell_size);
	if (cell_col < 0 || cell_col >= cols)
		return (-1);
	if (vars->map[cell_row][cell_col] == '1')
		return (-1);
	int cell_row2 = (int)((new_y + 3) / vars->cell_size);
	int cell_col2 = (int)((new_x + 3) / vars->cell_size);
	if (cell_row2 < 0 || cell_row2 >= rows || cell_col2 < 0 || cell_col2 >= cols)
		return (-1);
	if (vars->map[cell_row2][cell_col2] == '1')
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
	/* set intended forward/back and strafing based on keys state */
	if (vars->keys.w)
		ver -= vars->move_step;
	if (vars->keys.s)
		ver += vars->move_step;
	if (vars->keys.a)
		side -= vars->move_step;
	if (vars->keys.d)
		side += vars->move_step;
	/* apply rotation keys if desired (left/right) */
	if (vars->keys.left && vars->keys.right)
		; /* no rotation if both pressed */
	else
	{
		if (vars->keys.left)
			rotate(vars, -5);
		if (vars->keys.right)
			rotate(vars, 5);
	
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

	if (validate_move(vars, delta_y, delta_x) == 0)
	{
		vars->player->y += delta_y;
		vars->player->x += delta_x;
		draw_field_of_view(vars);
	}
	return (0);
}
