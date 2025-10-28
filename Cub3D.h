/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 17:27:32 by kbossio           #+#    #+#             */
/*   Updated: 2025/10/28 15:55:05 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "minilibx/mlx.h"
#include "libft/libft.h"

#ifndef CUB3D_H
# define CUB3D_H

typedef struct s_parsing {
    char	**map;
    int		map_h;
    int     pl_x;
    int     pl_y;
    char    pl_dir;
}	t_parsing;

typedef struct s_vars{
	void	    *mlx_ptr;
	void	    *win_ptr;
    t_parsing    *parsing;
}	t_vars;

int parsing(t_parsing *parsing, char *map_file);
int mat_free(char **matrix);

#endif