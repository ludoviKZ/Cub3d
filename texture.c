/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 18:00:07 by kbossio           #+#    #+#             */
/*   Updated: 2025/11/11 19:00:39 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int load_texture(t_vars *vars, t_texture *texture, int tex_index)
{
    int tex_w;
    int tex_h;

    tex_w = 64;
    tex_h = 64;
    if (!vars || !vars->parsing || !vars->parsing->textures)
        return (-1);
    if (tex_index < 0)
        return (-1);
    char *path = vars->parsing->textures[tex_index];
    if (!path)
        return (-1);
    texture->img[0] = mlx_xpm_file_to_image(vars->mlx, path, &tex_w, &tex_h);
    if (!texture->img[0])
        return (-1);
    texture->width = tex_w;
    texture->height = tex_h;
    texture->addr = mlx_get_data_addr(texture->img[0], &texture->bits_per_pixel,
                                      &texture->line_length, &texture->endian);
    if (!texture->addr)
    {
        mlx_destroy_image(vars->mlx, texture->img[0]);
        return (-1);
    }
    return (0);
}

int get_pixel(t_texture *tex, int x, int y)
{
    if (!tex || x < 0 || y < 0 || x >= tex->width || y >= tex->height)
        return (0);
    char *pixel = tex->addr + (y * tex->line_length + x * (tex->bits_per_pixel / 8));
    return (*(int *)pixel);
}

void draw_wall(void *mlx, void *win, t_texture *texture, int screen_x, int slice_height, double wallX)
{
    if (!texture || !texture->addr)
        return;
    int tex_w = texture->width;
    int tex_h = texture->height;

    int top = (MAP_HEIGHT / 2) - (slice_height / 2);
    int bottom = top + slice_height - 1;

    /* visible draw range (clip to screen) */
    int draw_top = (top < 0) ? 0 : top;
    int draw_bottom = (bottom >= MAP_HEIGHT) ? (MAP_HEIGHT - 1) : bottom;

    /* create a temporary 1px-wide image for this column */
    void *col_img = mlx_new_image(mlx, 1, MAP_HEIGHT);
    if (col_img)
    {
        int col_bpp = 0;
        int col_line_len = 0;
        int col_endian = 0;
        char *col_addr = mlx_get_data_addr(col_img, &col_bpp, &col_line_len, &col_endian);

        /* Use the full wall height (slice_height) when mapping texture coordinates
           so the texture sampling keeps correct perspective. Only fill the
           visible rows (draw_top..draw_bottom). */
        int full_wall_h = slice_height;
        for (int y = draw_top; y <= draw_bottom; ++y) {
            float tex_yf;
            if (full_wall_h > 1)
                tex_yf = ((float)(y - top) / (float)(full_wall_h - 1)) * (float)(tex_h - 1);
            else
                tex_yf = 0.0f;
            int tex_y = (int)(tex_yf + 0.5f);
            if (tex_y < 0) tex_y = 0;
            if (tex_y >= tex_h) tex_y = tex_h - 1;

            /* Compute horizontal texture coordinate from wallX fractional position */
            int tex_x = (int)(wallX * (double)tex_w);
            if (tex_x < 0) tex_x = 0;
            if (tex_x >= tex_w) tex_x = tex_w - 1;
            int color = get_pixel(texture, tex_x, tex_y);

            /* write into the column image buffer */
            char *dst = col_addr + (y * col_line_len);
            *(unsigned int *)dst = (unsigned int)color;
        }

        /* blit the column to the window at x=screen_x */
        mlx_put_image_to_window(mlx, win, col_img, screen_x, 0);
        mlx_destroy_image(mlx, col_img);
    }
    /* If image creation failed, do not draw any pixels (only use image blit).
       This intentionally omits any fallback to mlx_pixel_put as requested. */
    return;
}

void draw_texture(void *mlx, void *win, t_vars *vars, int ray, int slice_height, double wallX, int tex_index)
{
    t_texture *texture;
	(void)mlx;
	(void)win;

    texture = malloc(sizeof(t_texture));
    if (!texture)
        return;
    if (load_texture(vars, texture, tex_index) != 0)
    {
        free(texture);
        return;
    }

    draw_wall(vars->mlx, vars->win, texture, ray, slice_height, wallX);

    /* Destroy the image we created for this slice to avoid leaking X11 resources */
    if (texture->img[0])
        mlx_destroy_image(vars->mlx, texture->img[0]);
    free(texture);
}