/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:35:35 by kbossio           #+#    #+#             */
/*   Updated: 2025/12/05 17:59:43 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <math.h>
# include "libft/libft.h"
# include "minilibx/mlx.h"

# define MAP_WIDTH 1024
# define MAP_HEIGHT 512
# define M_PI 3.14159265358979323846
# define WALL_SCALE 100
# define CEILING_COLOR 0x87CEEB 
# define FLOOR_COLOR   0x444444 
# define CELL_SIZE 90
/* Size of one map cell in pixels. Reduce this to make the perceived map
	translation (movement and map scaling) smaller. */

	/*
	1111111111111111111
	11000000000000E0000
	1000000100000100001
	1000001110001101001
	1000000100000011001
	100000111000000001
	1111111  11111111
	1     111111111111
	111111111111111111	*/

typedef struct s_player
{
	double	x;
	double	y;
	int		dir;
}	t_player;

typedef struct s_parsing
{
	char	**map;
	char	**textures;
	int		map_h;
	int		pl_x;
	int		pl_y;
	char	pl_dir;
	int		*colors;
}	t_parsing;

typedef struct s_texture
{
	int		tex_i[4];
	void	*img[4];
	int		width;
	int		height;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	char	*col_addr;
	int		col_bpp;
	int		col_line_len;
	int		col_endian;
	char	*dst;
	int		top;
	int		bottom;
	int		draw_top;
	int		draw_bottom;
	int		tex_index;
}	t_texture;

typedef struct s_line2D
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	start;
	int	end;
}	t_line2D;

typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	left;
	int	right;
}	t_keys;

typedef struct s_vars
{
	void		*mlx;
	void		*win;
	char		**map;
	void		*background;
	int			fd;
	t_parsing	*parsing;
	t_player	*player;
	t_texture	*texture;
	t_keys		keys;
	double		wall_scale;
	int			move_step;
	int			cell_size;
}	t_vars;

typedef struct s_moveval
{
	double		new_x;
	double		new_y;
	int			rows;
	int			cell_row;
	int			cell_col;
	int			cell_row2;
	int			cell_col2;
	int			cell_row3;
	int			cell_col3;
}	t_moveval;

typedef struct s_movecalc
{
	double	rad;
	double	dx_f;
	double	dy_f;
	double	dx_s;
	double	dy_s;
	double	delta_x;
	double	delta_y;
}	t_movecalc;

typedef struct s_raycast
{
	int		fov_angle;
	int		num_rays;
	int		max_length;
	double	start_angle;
	double	angle_step;
	int		px;
	int		py;
	int		ray_index;
	double	ray_angle;
	double	rad;
	double	raydirx;
	double	raydiry;
	double	posx;
	double	posy;
	int		mapx;
	int		mapy;
	double	deltadistx;
	double	deltadisty;
	double	sidedistx;
	double	sidedisty;
	int		stepx;
	int		stepy;
	int		hit;
	int		side;
	double	perpwalldist;
	double	walldistpixels;
	int		hit_px;
	int		hit_py;
	int		map_rows;
}	t_raycast;

// Kris
int		parsing(t_parsing *parsing, char *map_file);
void	cleanup(t_vars *vars);
int		line_count(char *map_file);
int		mat_free(char **matrix);
int		check_textures(t_parsing *p);
char	**dup_matrix(char **map, t_parsing *p);
void	draw_texture(t_vars *vars, int ray, int slice_height, double wallX);
int		rgb_to_color(char **rgb);

// Ludo
int		init_player(t_vars *vars);
int		key_hook(int keycode, t_vars *vars);
int		key_release(int keycode, t_vars *vars);
int		close_window(t_vars *vars);
int		move(void *param);
void	rotate(t_vars *vars, int angle);
int		validate_move(t_vars *vars, double *ver, double *side);
void	draw_field_of_view(t_vars *vars);
int		dir_e_move(t_vars *vars, int ver, int side);
void	draw_3D_wall_slice(t_vars *vars, t_raycast *r, double wallX);
void	set_player_dir(t_vars *vars, char dir);
int		check_for_close_walls(t_vars *vars, t_moveval move);

/* Raycast helper functions (refactor targets) */
void	compute_ray_direction(t_vars *vars, t_raycast *r, int index);
void	setup_dda(t_vars *vars, t_raycast *r);
void	setup_dda_extra(t_vars *vars, t_raycast *r);
void	compute_hit_and_pixels(t_vars *vars, t_raycast *r);
double	compute_delta_dist(double rayDir);
int		slice_height_calc(t_vars *vars, double perpWallDist);
int		validate_move_cell(t_vars *vars, double x, double y);
void	setup_dda_extra(t_vars *vars, t_raycast *r);
void	setup_dda_extra_extra(t_raycast *r);
double	safe_denominator(double v);
void	compute_vars_setup(t_vars *vars, t_raycast *r, double *wallX);

#endif