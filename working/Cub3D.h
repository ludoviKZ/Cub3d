/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:35:35 by kbossio           #+#    #+#             */
/*   Updated: 2026/01/18 17:28:42 by kbossio          ###   ########.fr       */
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
	int		width[4];
	int		height[4];
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
	int			cr;
	int			cc;
	int			cr2;
	int			cc2;
	int			cr3;
	int			cc3;
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
int			parsing(t_parsing *parsing, char *map_file);
int			check_player(t_parsing *p, char **map, int i, int j);
int			flood_fill(char **map, int x, int y);
void		cleanup(t_vars *vars);
int			check_extension(char *filename);
t_parsing	*parsing_init(char **argv);
t_vars		*init_vars(t_parsing *p);
int			line_count(char *map_file);
int			mf(char **matrix, char *unused);
void		free_textures(char **textures);
int			check_textures(t_parsing *p);
int			check_tex(char *l, int *cont, t_parsing *p);
int			syntax_check_rgb(t_parsing *parsing, char **textures);
char		**dup_matrix(char **map);
void		draw_texture(t_vars *vars, int ray, int slice_height, double wallX);
int			rgb_to_color(char **rgb);
int			has_xpm_ext(const char *s);
int			skip_spaces(char *line, int *index);
int			check_matr(char **str);
int			check_lines(char **map);
char		**pad_map(char **map);
int			get_max_width(char **map);
char		*pad_line(char *line, int max_width);
int			check_borders_and_player(t_parsing *p, char **tmp, int i, int j);
int			validate_map_content(t_parsing *p, char **tmp);
int			dup_and_set_map(t_parsing *p, int i);
void		free_dup_matrix(char **new_map, int count);
int			handle_parsing_error(t_parsing *p, t_vars *vars);
void		destroy_partial_textures(t_vars *vars, t_texture *t, int count);
int			set_texture(t_parsing *p, int i, char *l, int *cont);
void		free_init_player(t_vars *vars);
void		print_map(char **map);

// Ludo
int			init_player(t_vars *vars);
int			key_hook(int keycode, t_vars *vars);
int			key_release(int keycode, t_vars *vars);
int			close_window(t_vars *vars);
int			move(void *param);
void		rotate(t_vars *vars, int angle);
int			validate_move(t_vars *vars, double *ver, double *side);
void		draw_field_of_view(t_vars *vars);
int			dir_e_move(t_vars *vars, int ver, int side);
void		draw_3d_wall_slice(t_vars *vars, t_raycast *r, double wallX);
void		set_player_dir(t_vars *vars, char dir);
int			init_texture(t_vars *vars, t_texture *t, t_parsing *p);
int			init_texture_values(t_texture *texture);
int			check_for_close_walls(t_vars *vars, t_moveval move);
void		tex_set(int *tex_y, int *tex_x, t_vars *v);
void		compute_ray_direction(t_vars *vars, t_raycast *r, int index);
void		setup_dda(t_vars *vars, t_raycast *r);
void		setup_dda_extra(t_vars *vars, t_raycast *r);
void		compute_hit_and_pixels(t_vars *vars, t_raycast *r);
double		compute_delta_dist(double rayDir);
int			slice_height_calc(t_vars *vars, double perpWallDist);
int			validate_move_cell(t_vars *vars, double x, double y);
void		setup_dda_extra_extra(t_raycast *r);
double		safe_denominator(double v);
void		compute_vars_setup(t_vars *vars, t_raycast *r, double *wallX);

#endif