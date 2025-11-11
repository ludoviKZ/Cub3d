#include <math.h>
#include "Cub3D.h"

/* draw_field_of_view was refactored into raycast.c / raycast_extra.c.
   The implementation was moved to keep player.c focused on player logic. */

int slice_height_calc(t_vars *vars, double perpWallDist)
{
	double corrected_dist_pixels;

	/* perpWallDist is already the perpendicular distance (in map cells)
	   computed by the DDA. Convert to pixels and use directly — no extra
	   fisheye cosine correction required here. */
	corrected_dist_pixels = perpWallDist * (double)vars->cell_size;
	if (corrected_dist_pixels < 0.0001)
		corrected_dist_pixels = 0.0001;
	if (corrected_dist_pixels < 1.0)
		corrected_dist_pixels = 1.0;
	return (int)((MAP_HEIGHT * vars->wall_scale) / corrected_dist_pixels);
}
void draw_3D_wall_slice(t_vars *vars, int ray, int px, int py, double perpWallDist, double ray_angle)
{
	int slice_height;
	int top;
	int bottom;
	int wall_color;
	(void)px;
	(void)py;
	(void)ray_angle;

	slice_height = slice_height_calc(vars, perpWallDist);
	if (slice_height > MAP_HEIGHT)
		slice_height = MAP_HEIGHT;
	if (slice_height < 0)
		slice_height = 0;
	top = (MAP_HEIGHT / 2) - (slice_height / 2);
	bottom = top + slice_height - 1;
	if (top < 0)
		top = 0;
	if (bottom >= MAP_HEIGHT)
		bottom = MAP_HEIGHT - 1;
	if (top > 0)
		draw_line2D(vars->mlx, vars->win, ray, 0, ray, top - 1, CEILING_COLOR, 1);
	wall_color = 0x808080;
	if (bottom >= top)
		draw_line2D(vars->mlx, vars->win, ray, top, ray, bottom, wall_color, 1);

	if (bottom < MAP_HEIGHT - 1)
		draw_line2D(vars->mlx, vars->win, ray, bottom + 1, ray, MAP_HEIGHT - 1, FLOOR_COLOR, 1);
	return;
}
