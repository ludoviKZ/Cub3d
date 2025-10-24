/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:52:52 by kbossio           #+#    #+#             */
/*   Updated: 2025/10/24 18:53:15 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3d.h"

int mat_free(char **matrix)
{
    int i;

    i = 0;
    if (!matrix)
        return (0);
    while (matrix[i])
    {
        free(matrix[i]);
        i++;
    }
    free(matrix);
    return (0);
}