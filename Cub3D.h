
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

typedef struct s_vars
{
	void	*mlx;
	void	*win;
	char	**map;
	int		fd;
	t_parsing    *parsing;
	t_player	*player;
}	t_vars;

// Kris
int parsing(t_parsing *parsing, char *map_file);
int parsing_init(t_parsing *parsing);
int line_count(char *map_file);
int mat_free(char **matrix);
int check_textures(t_parsing *p);
int dup_matrix(char **map, t_parsing *p);


// Ludo
int		draw_square2D(char **map, int size[], void *mlx_ptr, void *win_ptr);
void	draw_square(void *mlx_ptr, void *win_ptr, int x0, int y0, int size, int color);
int		init_player(t_vars *vars);
void	move(t_vars *vars, int ver, int side);
void	rotate(t_vars *vars, int angle);
void draw_line2D(void *mlx_ptr, void *win_ptr, int x1, int y1, int x2, int y2, int color, int thickness);
int validate_move(t_vars *vars, int ver, int side);
void draw_field_of_view(t_vars *vars, int color);
void check_line_collision(t_vars *vars, int x1, int y1, int *x2, int *y2);
void	redraw_scene(t_vars *vars);

#endif