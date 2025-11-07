#include "Cub3D.h"

/* Compute ray direction for a given ray index. Variables are declared at the
   top and assigned on separate lines to meet the requested style. */
void compute_ray_direction(t_vars *vars, t_raycast *r, int index)
{
    (void)vars;
    r->ray_angle = r->start_angle + ((double)index * r->angle_step);
    r->rad = r->ray_angle * (M_PI / 180.0);
    r->rayDirX = cos(r->rad);
    r->rayDirY = sin(r->rad);
}

/* Top-level draw function: iterates rays using a while loop (no for loops).
   It orchestrates the per-ray helpers placed across files. */
void draw_field_of_view(t_vars *vars)
{
    t_raycast   r;
    int         i;

    if (!vars || !vars->player || !vars->map)
        return;

    r.fov_angle = 60;
    r.num_rays = MAP_WIDTH;
    r.max_length = 400;
    r.start_angle = (double)vars->player->dir - ((double)r.fov_angle / 2.0);
    r.angle_step = (double)r.fov_angle / (double)r.num_rays;
    r.px = vars->player->x + 2;
    r.py = vars->player->y + 2;
    r.map_rows = 0;
    while (vars->map[r.map_rows])
        r.map_rows++;
    i = 0;
    while (i < r.num_rays)
    {
        r.ray_index = i;
        compute_ray_direction(vars, &r, i);
        setup_dda(vars, &r);
        compute_hit_and_pixels(vars, &r);
        i++;
    }
}
