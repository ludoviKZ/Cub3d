/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 18:00:07 by kbossio           #+#    #+#             */
/*   Updated: 2025/11/07 17:03:14 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int load_texture(t_vars *vars, t_texture *texture)
{
    int tex_w;
    int tex_h;

    tex_w = 64;
    tex_h = 64;
    texture->img[0] = mlx_xpm_file_to_image(vars->mlx, "textures/north.xpm", &tex_w, &tex_h);
    if (!texture->img[0])
        return (-1);
    texture->width = tex_w;
    texture->height = tex_h;
    texture->addr = mlx_get_data_addr(texture->img[0], &texture->bits_per_pixel,
                                      &texture->line_length, &texture->endian);
    if (!texture->addr)
        return (-1);
    return (0);
}

int get_pixel(t_texture *tex, int x, int y)
{
    if (!tex || x < 0 || y < 0 || x >= tex->width || y >= tex->height)
        return (0);
    char *pixel = tex->addr + (y * tex->line_length + x * (tex->bits_per_pixel / 8));
    return (*(int *)pixel);
}

void my_mlx_pixel_put(t_texture *tex, int x, int y, int color)
{
    if (!tex || x < 0 || y < 0 || x >= tex->width || y >= tex->height)
        return;
    char *dst = tex->addr + (y * tex->line_length + x * (tex->bits_per_pixel / 8));
    *(int *)dst = color;
}

void draw_skewed_image(void *mlx, void *win, t_texture *src, float perspective)
{
    /* Axis-swapped perspective: same trapezoid idea but applied across X.
       As x increases the vertical span shrinks (columns become 'farther').
       'perspective' is the fraction of full height at the left side (0..1).
    */
    if (!src || !src->addr)
        return;
    int dst_w = src->width;
    int dst_h = src->height;
    t_texture dst;
    dst.width = dst_w;
    dst.height = dst_h;
    dst.img[0] = mlx_new_image(mlx, dst.width, dst.height);
    if (!dst.img[0])
        return;
    dst.addr = mlx_get_data_addr(dst.img[0], &dst.bits_per_pixel, &dst.line_length, &dst.endian);

     /* perspective is the fraction of full height at the RIGHT side (0..1).
         Values <1.0 make the image shrink as x increases (toward the right). */
     float right_scale = perspective;
     if (right_scale < 0.01f) right_scale = 0.01f;
     if (right_scale > 1.0f) right_scale = 1.0f;

    float dst_cy = (dst_h - 1) * 0.5f;

    /* For each destination column, compute allowed vertical span and map into source */
    for (int x = 0; x < dst_w; ++x) {
        float tx = (dst_w > 1) ? ((float)x / (dst_w - 1)) : 0.0f;
    /* current scale: 1.0 at left -> right_scale at right (shrinks as x increases) */
    float cur_scale = 1.0f - (1.0f - right_scale) * tx;
        float cur_h = (float)src->height * cur_scale;
        float top = dst_cy - (cur_h * 0.5f);
        float bottom = top + cur_h;

        /* map column x to a source x (linear across width) */
        float src_xf = tx * (float)(src->width - 1);
        int src_x = (int)(src_xf + 0.5f);
        if (src_x < 0) src_x = 0;
        if (src_x >= src->width) src_x = src->width - 1;

        for (int y = 0; y < dst_h; ++y) {
            if ((float)y < top || (float)y >= bottom)
                continue;
            /* inverse-map vertical position within this column to source y */
            float local_y = (float)(y - top);
            float src_yf = (local_y / cur_h) * (float)(src->height - 1);
            int src_y = (int)(src_yf + 0.5f);
            if (src_y < 0) src_y = 0;
            if (src_y >= src->height) src_y = src->height - 1;
            int color = get_pixel(src, src_x, src_y);
            my_mlx_pixel_put(&dst, x, y, color);
        }
    }

    mlx_put_image_to_window(mlx, win, dst.img[0], 64, 64);
    mlx_destroy_image(mlx, dst.img[0]);
}

void draw_texture(t_vars *vars)
{
    t_texture *texture;

    texture = malloc(sizeof(t_texture));
    if (!texture)
        return;
    load_texture(vars, texture);
    // mlx_put_image_to_window(vars->mlx, vars->win, texture->img[0], 0, 0);
    draw_skewed_image(vars->mlx, vars->win, texture, 0.5f);
    vars->texture = texture;
}