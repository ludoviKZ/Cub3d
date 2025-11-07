
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

#define MAP_WIDTH 1024
#define MAP_HEIGHT 512
# define M_PI 3.14159265358979323846
#define WALL_SCALE 4.0
#define CEILING_COLOR 0x87CEEB 
#define FLOOR_COLOR   0x444444 
/* Size of one map cell in pixels. Reduce this to make the perceived map
	translation (movement and map scaling) smaller. */
#define CELL_SIZE 30

typedef struct s_player
{
	int	x;
	int	y;
	int	dir;
}	t_player;

typedef struct s_parsing {
    char	**map;
	char	**textures;
    int		map_h;
    int     pl_x;
    int     pl_y;
    char    pl_dir;
}	t_parsing;

typedef struct s_texture {
	void	*img[4];
	int		width;
	int		height;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_texture;

typedef struct s_vars
{
	void	*mlx;
	void	*win;
	char	**map;
	int		fd;
	t_parsing    *parsing;
	t_player	*player;
	t_texture	*texture;
	/* Runtime adjustable parameters */
	double  wall_scale; /* current wall scale (initialized from WALL_SCALE) */
	int     move_step;  /* pixels moved per step */
	int     cell_size;  /* runtime cell size (initialized from CELL_SIZE macro) */
}	t_vars;

/* Raycasting helper structure: groups temporary variables used while
   casting a single frame. This keeps draw functions' locals in one place
   so the code can be split across files easily. */
typedef struct s_raycast
{
	int     fov_angle;
	int     num_rays;
	int     max_length;
	double  start_angle;
	double  angle_step;
	int     px;
	int     py;
	int     ray_index;
	double  ray_angle;
	double  rad;
	double  rayDirX;
	double  rayDirY;
	double  posX;
	double  posY;
	int     mapX;
	int     mapY;
	double  deltaDistX;
	double  deltaDistY;
	double  sideDistX;
	double  sideDistY;
	int     stepX;
	int     stepY;
	int     hit;
	int     side;
	double  perpWallDist;
	double  wallDistPixels;
	int     hit_px;
	int     hit_py;
	int     map_rows;
}   t_raycast;

// Kris
int parsing(t_parsing *parsing, char *map_file);
int parsing_init(t_parsing *parsing);
int line_count(char *map_file);
int mat_free(char **matrix);
int check_textures(t_parsing *p);
char **dup_matrix(char **map, t_parsing *p);
void draw_texture(t_vars *vars);


// Ludo
int		draw_square2D(char **map, int size[], void *mlx_ptr, void *win_ptr);
void	draw_square(void *mlx_ptr, void *win_ptr, int x0, int y0, int size, int color);
int		init_player(t_vars *vars);
void	move(t_vars *vars, int ver, int side);
void	rotate(t_vars *vars, int angle);
void draw_line2D(void *mlx_ptr, void *win_ptr, int x1, int y1, int x2, int y2, int color, int thickness);
int validate_move(t_vars *vars, int ver, int side);
void draw_field_of_view(t_vars *vars);
void check_line_collision(t_vars *vars, int x1, int y1, int *x2, int *y2);
void	redraw_scene(t_vars *vars);
void draw_3D_wall_slice(t_vars *vars, int ray, int px, int py, int hit_x, int hit_y, double ray_angle);

/* Raycast helper functions (refactor targets) */
void    compute_ray_direction(t_vars *vars, t_raycast *r, int index);
void    setup_dda(t_vars *vars, t_raycast *r);
void    compute_hit_and_pixels(t_vars *vars, t_raycast *r);
double  compute_delta_dist(double rayDir);

#endif