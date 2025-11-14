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
void draw_3D_wall_slice(t_vars *vars, int ray, int px, int py, double perpWallDist, double ray_angle, double wallX, int tex_index)
{
	int slice_height;
	int top;
	int bottom;
	int wall_color;
	(void)px;
	(void)py;
	(void)ray_angle;

	slice_height = slice_height_calc(vars, perpWallDist);
	/* Keep the calculated slice_height even if it's larger than the screen.
	   We'll only draw the visible rows — do not scale the wall to fit. */
	if (slice_height < 1)
		slice_height = 1;
	top = (MAP_HEIGHT / 2) - (slice_height / 2);
	bottom = top + slice_height - 1;
	if (top < 0)
		top = 0;
	if (bottom >= MAP_HEIGHT)
		bottom = MAP_HEIGHT - 1;
	/* Draw ceiling and floor for this column in a single call. Pass both
	   ceiling and floor parsed colors (parsing->colors[1] = ceiling,
	   parsing->colors[0] = floor). Keep a single blit per-column. */
	//draw_background(vars, ray, top, bottom, vars->parsing->colors[1], vars->parsing->colors[0]);
	// mlx_put_image_to_window(vars->mlx, vars->win, vars->background, ray, 0);
	wall_color = 0x808080;
	if (bottom >= top)
		draw_texture(vars, ray, slice_height, wallX, tex_index);

	/* floor is handled by the single draw_background call above */
	return;
}
