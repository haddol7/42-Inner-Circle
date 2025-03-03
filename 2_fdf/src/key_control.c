/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daeha <daeha@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:49:53 by daeha             #+#    #+#             */
/*   Updated: 2024/03/24 18:48:51 by daeha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	center_map(t_client *data)
{
	ft_memset(&data->map.angular, 0, sizeof(t_point));
	ft_memset(&data->map.translate, 0, sizeof(t_point));
	set_default_scale(&data->map);
}

int	key_hook(int keycode, t_client *data)
{
	if (keycode == KEY_A)
		data->map.angular.z -= 5;
	else if (keycode == KEY_D)
		data->map.angular.z += 5;
	else if (keycode == KEY_W)
		data->map.angular.x -= 5;
	else if (keycode == KEY_S)
		data->map.angular.x += 5;
	else if (keycode == KEY_Q)
		data->map.angular.y -= 5;
	else if (keycode == KEY_E)
		data->map.angular.y += 5;
	else if (keycode == KEY_C)
		center_map(data);
	else if (keycode == KEY_ESC)
		terminate((void *)data);
	else if (keycode == KEY_P || keycode == KEY_I)
		data->map.key = keycode;
	draw(data->map, &data->img, data->mlx, data->win);
	return (0);
}

int	mouse_press_hook(int keycode, int x, int y, t_client *data)
{
	if (data->mouse.is_pressed != 0)
		return (0);
	if (keycode == MOUSE_LEFT)
	{
		data->map.mouse.is_pressed = 1;
		data->mouse.is_pressed = 1;
		data->mouse.x = y;
		data->mouse.y = x;
	}
	else if (keycode == MOUSE_RIGHT)
	{
		data->map.mouse.is_pressed = 2;
		data->mouse.is_pressed = 2;
		data->mouse.x = x;
		data->mouse.y = y;
	}
	else if (keycode == MOUSE_UP && data->map.scale < 600)
		data->map.scale *= 1.2;
	else if (keycode == MOUSE_DOWN && data->map.scale > 0.1)
		data->map.scale *= 0.5;
	draw(data->map, &data->img, data->mlx, data->win);
	return (0);
}

int	mouse_drag_hook(int x, int y, t_client *data)
{
	if (data->mouse.is_pressed == 1)
	{
		data->map.mouse.is_pressed = 1;
		data->map.mouse.x = y - data->mouse.x;
		data->map.mouse.y = data->mouse.y - x;
	}
	else if (data->mouse.is_pressed == 2)
	{
		data->map.mouse.is_pressed = 2;
		data->map.mouse.x = x - data->mouse.x;
		data->map.mouse.y = y - data->mouse.y;
	}
	draw(data->map, &data->img, data->mlx, data->win);
	return (0);
}

int	mouse_release_hook(int keycode, int x, int y, t_client *data)
{
	if (keycode != data->mouse.is_pressed)
		return (0);
	if (keycode == MOUSE_LEFT || keycode == MOUSE_RIGHT)
	{
		data->mouse.is_pressed = 0;
		data->map.mouse.is_pressed = 0;
		if (keycode == MOUSE_LEFT)
		{
			data->map.angular.x += data->map.mouse.x;
			data->map.angular.y += data->map.mouse.y;
		}
		else
		{
			data->map.translate.x += data->map.mouse.x;
			data->map.translate.y += data->map.mouse.y;
		}
		data->map.mouse.x = 0;
		data->map.mouse.y = 0;
	}
	if (x * y == 42)
		ft_printf("\r🥚\r");
	draw(data->map, &data->img, data->mlx, data->win);
	return (0);
}
