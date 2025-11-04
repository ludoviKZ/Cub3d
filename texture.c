/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 18:00:07 by kbossio           #+#    #+#             */
/*   Updated: 2025/11/04 18:49:19 by kbossio          ###   ########.fr       */
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
    /* Simple trapezoid perspective: top of the image is narrower (farther) and
       bottom is full width (near). 'perspective' is the top width scale in
       [0.0,1.0] (e.g. 0.25 => top is 25% of full width). This is a fast,
       scanline-based inverse mapping (dest->src) that produces a convincing
       perspective effect without a full homography. */
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

    float top_scale = perspective; /* fraction of width at the very top */
    if (top_scale < 0.01f) top_scale = 0.01f;
    if (top_scale > 1.0f) top_scale = 1.0f;

    float dst_cx = (dst_w - 1) * 0.5f;

    for (int y = 0; y < dst_h; ++y) {
        float t = (dst_h > 1) ? ((float)y / (dst_h - 1)) : 0.0f;
        /* current horizontal scale: interpolates from top_scale -> 1.0 */
        float cur_scale = top_scale + (1.0f - top_scale) * t;
        float cur_w = src->width * cur_scale;
        float left = dst_cx - (cur_w * 0.5f);

        for (int x = 0; x < dst_w; ++x) {
            /* only map pixels that fall inside the trapezoid for this scanline */
            if ((float)x < left || (float)x >= left + cur_w)
                continue;
            /* inverse-map destination x into source x (horizontal distortion only) */
            float src_xf = ((float)(x - left) / cur_w) * (float)(src->width - 1);
            /* map y linearly (no vertical stretch) */
            float src_yf = t * (float)(src->height - 1);
            int src_x = (int)(src_xf + 0.5f);
            int src_y = (int)(src_yf + 0.5f);
            if (src_x < 0) src_x = 0;
            if (src_x >= src->width) src_x = src->width - 1;
            if (src_y < 0) src_y = 0;
            if (src_y >= src->height) src_y = src->height - 1;
            int color = get_pixel(src, src_x, src_y);
            my_mlx_pixel_put(&dst, x, y, color);
        }
    }

    mlx_put_image_to_window(mlx, win, dst.img[0], 64, 64);
    mlx_destroy_image(mlx, dst.img[0]);
}

// dst_w, dst_h chosen as desired (e.g., window area)
void draw_perspective_image(void *mlx, void *win, t_texture *src,
                            int dst_x, int dst_y,
                            int dst_w, int dst_h,
                            float top_scale, float bottom_scale)
{
    if (!src || !src->addr) return;
    t_texture dst;
    dst.width  = dst_w;
    dst.height = dst_h;
    dst.img[0] = mlx_new_image(mlx, dst.width, dst.height);
    if (!dst.img[0]) return;
    dst.addr = mlx_get_data_addr(dst.img[0], &dst.bits_per_pixel, &dst.line_length, &dst.endian);

    float src_cx = (src->width - 1) * 0.5f;
    // float src_cy = (src->height - 1) * 0.5f;
    float dst_cx = (dst.width - 1) * 0.5f;
    // float dst_cy = (dst.height - 1) * 0.5f;

    for (int y = 0; y < dst.height; ++y)
    {
        float t = (dst.height > 1) ? ((float)y / (dst.height - 1)) : 0.0f;
        float scale = top_scale + (bottom_scale - top_scale) * t;
        float inv_scale = 1.0f / scale;

        // map vertical coordinate linearly (you can tweak this)
        float src_yf = (float)y * (float)src->height / (float)dst.height;
        int src_y = (int)(src_yf + 0.5f);
        if (src_y < 0) src_y = 0;
        if (src_y >= src->height) src_y = src->height - 1;

        for (int x = 0; x < dst.width; ++x)
        {
            // inverse map x:
            float src_xf = src_cx + ( (x - dst_cx) * inv_scale );
            int src_x = (int)(src_xf + 0.5f);
            if (src_x < 0 || src_x >= src->width) continue; // skip outside
            int color = get_pixel(src, src_x, src_y);
            my_mlx_pixel_put(&dst, x, y, color);
        }
    }

    mlx_put_image_to_window(mlx, win, dst.img[0], dst_x, dst_y);
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