#include "libft/libft.h"
#include "minilibx/mlx.h"


int draw_square2D(char **map, int size[], void *mlx_ptr, void *win_ptr)
{
	int x;
	int y;
	int square_size;

	square_size = 15; // Define the size of each square
	y = 0;
	while (y < size[1])
	{
		x = 0;
		while (x < size[0])
		{
			if (map[y][x] == '1') // Assuming '1' represents a wall
			{
				// Draw a square at (x * square_size, y * square_size)
				// with dimensions square_size x square_size
				// and a specific color for walls
				draw_filled_square_mlx_pixel_put(mlx_ptr, win_ptr, x * square_size, y * square_size, square_size, 70);
			}
			else if (map[y][x] == '0') // Assuming '0' represents empty space
			{
				// Draw a square for empty space
				draw_filled_square_mlx_pixel_put(mlx_ptr, win_ptr, x * square_size, y * square_size, square_size, 120);
			}
			// Add more conditions for different map elements as needed
			x++;
		}
		y++;
	}
	return 0;
}
void draw_filled_square_mlx_pixel_put(void *mlx_ptr, void *win_ptr, int x0, int y0, int size, int color)
{
    int x;
    int y;

    y = 0;
    while (y < size - 1)
    {
        x = 0;
        while (x < size - 1)
        {
            mlx_pixel_put(mlx_ptr, win_ptr, x0 + x, y0 + y, color);
            x++;
        }
        y++;
    }
}