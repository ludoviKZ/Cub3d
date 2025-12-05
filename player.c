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
	texture->dst = NULL;
	texture->top = 0;
	texture->bottom = 0;
	texture->draw_top = 0;
	texture->draw_bottom = 0;
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
	set_player_dir(vars, vars->parsing->pl_dir);
	vars->wall_scale = WALL_SCALE;
	draw_field_of_view(vars); // draw 3D projection across the screen
	return (0);
}
void set_player_dir(t_vars *vars, char dir)
{
	if (dir == 'N')
		vars->player->dir = 270;
	else if (dir == 'S')
		vars->player->dir = 90;
	else if (dir == 'E')
		vars->player->dir = 180;
	else if (dir == 'W')
		vars->player->dir = 0;
	else
		vars->player->dir = 0;
}
