/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_speci.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daeha <daeha@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 21:36:33 by daeha             #+#    #+#             */
/*   Updated: 2024/01/31 01:09:25 by daeha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int make_speci_c(t_spec *spec, t_flag flag, va_list *ap)
{
	char	*str;
	char	arg;

	arg = va_arg(*ap, int);
	str = (char *)malloc(sizeof(char) * 2);
	if (str == NULL)
		return (ERROR);
	str[0] = arg;
	str[1] = '\0';
	spec->str = str;
	spec->type = 'c';
	spec->size = 1;
	return (make_field_c_p(spec, flag));
}

int make_speci_s(t_spec *spec, t_flag flag, va_list *ap)
{
	char	*str;
	char	*arg;

	arg = va_arg(*ap, char *);
	if (arg == NULL)
		str = ft_strdup("(null)", &spec->size);
	else
		str = ft_strdup(arg, &spec->size);
	if (str == NULL)
		return (ERROR);
	spec->str = str;
	spec->type = 's';
	return (make_field_s(spec, flag));
}

static char	*make_speci_xX_p_getstr(unsigned long long arg, int *len, char type)
{
	char	*str;

	if (type == 'p')
		*len = 2;
	else
		*len = 0;
	if (arg == 0)
	{
		(*len)++;
		if (type == 'p')
			return (ft_strdup("0x0", len));
		else
			return (ft_strdup("0", len));	
	}
	while (arg != 0)
	{
		arg /= 16;
		(*len)++;
	}
	str = (char *)malloc((*len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	return (str);
}

int make_speci_p(t_spec *spec, t_flag flag, va_list *ap)
{
	int					len;
	char				*str;
	const char			*hex;
	unsigned long long	arg;

	hex = "0123456789abcdef";
	arg = (unsigned long long)va_arg(*ap, void *);
	str = make_speci_xX_p_getstr(arg, &len, 'p');
	if (str == NULL)
		return (ERROR);
	spec->size = len;
	str[0] = '0';
	str[1] = 'x';
	str[len--] = '\0';
	while (arg != 0 && len >= 0)
	{
		str[len--] = hex[arg % 16];
		arg /= 16;
	}
	spec->str = str;
	spec->type = 'p';
	return (make_field_c_p(spec, flag));
}

int make_speci_x_X(t_spec *spec, t_flag *flag, va_list *ap, char type)
{
	int					len;
	char				*str;
	const char			*hex;
	unsigned long long	arg;

	if (type == 'x')
		hex = "0123456789abcdef";
	else
		hex = "0123456789ABCDEF";
	arg = (unsigned long long)va_arg(*ap, unsigned int);
	if (arg == 0)
		flag->alt = 0;
	str = make_speci_xX_p_getstr(arg, &len, type);
	if (str == NULL)
		return (ERROR);
	spec->size = len;
	str[len--] = '\0';
	while (arg != 0 && len >= 0)
	{
		str[len--] = hex[arg % 16];
		arg /= 16;
	}
	spec->str = str;
	spec->type = type;
	return (make_field_x_X(spec, *flag));
}
