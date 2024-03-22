/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_load.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daeha <daeha@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 19:08:56 by daeha             #+#    #+#             */
/*   Updated: 2024/03/22 16:40:51 by daeha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "fdf.h"

static char	*read_map_as_one_line(int fd);
static void	check_map_size(char *content, t_map *map);
static void	allocate_map(char *content, t_map *map);
static void	set_scale(t_map *map);

// load_map(char *content, t_map *map)
//
// 	call read_map function to read .fdf files.
// 	when a file is given below,
// 	1	2	3
// 	4	5	6
// 	7	8	9
// 	map.point is initialized like
// 	map.point[0].x = 0 | map.point[0].y = 0 | map.point[0].z = 1
// 	map.point[1].x = 1 | map.point[1].y = 0 | map.point[1].z = 2
// 	map.point[2].x = 2 | map.point[2].y = 0 | map.point[2].z = 3
// 	
// 	and map.col and map.row are initialized by 3.
// 1. system-call function error
// 2. map file format error

void	load_map(char *dir, t_client *data)
{
	int		fd;
	char	*content;

	fd = open(dir, O_RDONLY);
	if (fd == -1)
		fdf_error(ERR_OPN);
	content = read_map_as_one_line(fd);
	check_map_size(content, &data->map);
	allocate_map(content, &data->map);
	free(content);
	set_scale(&data->map);
	draw(data->map, &data->img, data->mlx, data->win);
}

static char	*read_map_as_one_line(int fd)
{
	int		read_size;
	char	buffer[FDF_BUFFER_SIZE];
	char	*temp;
	char	*res;

	temp = ft_strdup("");
	res = temp;
	if (temp == NULL)
		fdf_error(ERR_MLC);
	read_size = read(fd, buffer, FDF_BUFFER_SIZE);
	while (read_size != 0)
	{
		if (read_size == -1)
			fdf_error(ERR_RD);
		buffer[read_size] = '\0';
		res = ft_strjoin(temp, buffer);
		if (res == NULL)
			fdf_error(ERR_MLC);
		free(temp);
		temp = res;
		read_size = read(fd, buffer, FDF_BUFFER_SIZE);
	}
	close(fd);
	return (res);
}

static void	check_map_size(char *s, t_map *map)
{
	size_t	x;
	size_t	y;

	x = 0;
	y = 0;
	map->col = 0;
	while (*s != '\0')
	{
		if (*s != ' ' && *s != '\n' && fdf_isspace(*(s + 1)))
			x++;
		if (*s == '\n')
		{
			if ((x != map->col && map->col != 0) || x == 0)
				fdf_error(ERR_M_SIZE);
			if (map->col == 0)
				map->col = x;
			x = 0;
			y++;
		}
		s++;
	}
	map->row = y;
	if (map->row * map->col == 0)
		fdf_error(ERR_M_EMPT);
}

static void	allocate_map(char *s, t_map *map)
{
	size_t			size;
	unsigned long	i;

	i = 0;
	size = map->col * map->row;
	map->point = malloc(sizeof(t_point) * size);
	if (!map->point)
		fdf_error(ERR_MLC);
	map->copy = malloc(sizeof(t_proj) * size);
	if (!map->copy)
		fdf_error(ERR_MLC);
	while (i < size)
	{
		while (!ft_isdigit(*s) && *s != '-')
		{
			if (!fdf_isspace(*s))
				fdf_error(ERR_M_VAL);
			s++;
		}
		map->point[i].x = i % map->col - map->col / 2;
		map->point[i].y = i / map->col - map->row / 2;
		map->point[i].z = check_map_value(&s, &map->point[i].color);
		i++;
	}
}

static void	set_scale(t_map *map)
{
	size_t	i;
	size_t	size;
	int		z_max;

	i = 0;
	z_max = -2147483648;
	size = map->col * map->row;
	if (map->col > map->row)
		map->scale = WINDOW_X_SIZE / (double)map->col;
	else
		map->scale = WINDOW_Y_SIZE / (double)map->row;
	if (map->scale < 0)
		map->scale = 0.1;
	while (i < size)
	{	
		if (z_max < map->point[i].z)
			z_max = map->point[i].z;
		i++;
	}
	i = 0;
	while (i < size)
	{
		map->point[i].z *= map->scale * 10 / z_max;
		i++;
	}
}
