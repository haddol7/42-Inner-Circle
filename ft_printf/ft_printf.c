/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daeha <daeha@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 18:57:58 by daeha             #+#    #+#             */
/*   Updated: 2024/01/31 16:59:00 by daeha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	write_literally(const char *format, int len, int *result)
{
	int	temp;

	temp = write(1, format, len);
	if (temp < 0)
		return (ERROR);
	*result += temp;
	return (1);
}

static int	write_conversion(const char *format, int *i, int *result, va_list *ap)
{
	t_field	field;
	t_flag	flag;

	if (format[*i] == '\0')
		return (OK);
	parse_flag(format, &flag, i);
	if (!make_conversion(&field, flag, ap, format[(*i)++]) || \
		!write_literally(field.str, field.size, result))
		return (ERROR);
	if (field.str != NULL)
		free(field.str);
	field.str = NULL;
	return (1);
}

int	ft_printf(const char *format, ...)
{
	int		result;
	int		offset;
	int		i;
	va_list	ap;

	result = 0;
	offset = 0;
	i = 0;
	va_start(ap, format);
	while (format[offset] != '\0')
	{
		if (format[offset + i] == '\0' || format[offset + i] == '%')
		{
			if (!write_literally(format + offset, i, &result) || \
				!write_conversion(format + offset, &i, &result, &ap))
				return (-1);
			offset += i;
			i = -1;
		}
		i++;
	}
	va_end(ap);
	return (result);
}
