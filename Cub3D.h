
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

typedef struct s_vars
{
	void	*mlx;
	void	*win;
	char	**map;
	int		fd;
	t_player	*player;
}	t_vars;


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