/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mskhairi <mskhairi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:18:02 by mskhairi          #+#    #+#             */
/*   Updated: 2024/08/05 11:05:36 by mskhairi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_format(char c, va_list pa)
{
	int	count;

	count = 0;
	if (c == 'c')
		count += ft_putchar_fd(va_arg(pa, int), 2);
	else if (c == 's')
		count += ft_putstr_fd(va_arg(pa, char *), 2);
	else
		count += ft_putchar_fd(c, 2);
	return (count);
}

int	ft_printf_error(const char *s, ...)
{
	int		i;
	va_list	args;
	int		count;

	i = 0;
	count = 0;
	va_start(args, s);
	while (s[i])
	{
		if (s[i] == '%')
		{
			if (s[i + 1] == '\0')
				return (count);
			count += ft_format(s[++i], args);
		}
		else
			count += ft_putchar_fd(s[i], 2);
		i++;
	}
	va_end(args);
	return (count);
}
