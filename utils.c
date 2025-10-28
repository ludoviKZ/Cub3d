/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbossio <kbossio@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:52:52 by kbossio           #+#    #+#             */
/*   Updated: 2025/10/28 18:46:10 by kbossio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cub3D.h"

int mat_free(char **matrix)
{
    int i;

    i = 0;
    if (!matrix || !matrix[0])
        return (0);
    while (matrix[i])
    {
        free(matrix[i]);
        i++;
    }
    free(matrix);
    return (0);
}