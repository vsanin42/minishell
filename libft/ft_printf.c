/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 17:53:56 by vsanin            #+#    #+#             */
/*   Updated: 2024/06/12 19:37:25 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_convert(va_list ptr, char format)
{
	if (format == 'c')
		return (ft_putchar(va_arg(ptr, int)));
	else if (format == 's')
		return (ft_putstr(va_arg(ptr, char *)));
	else if (format == 'p')
		return (ft_putptr(va_arg(ptr, void *)));
	else if (format == 'd' || format == 'i')
		return (ft_putnbr(va_arg(ptr, int)));
	else if (format == 'u')
		return (ft_putunsigned(va_arg(ptr, unsigned int)));
	else if (format == 'x' || format == 'X')
		return (ft_puthex(va_arg(ptr, unsigned int), format));
	else if (format == '%')
		return (ft_putchar('%'));
	else
		return (-1);
}

int	ft_printf(const char *format, ...)
{
	int		count;
	va_list	ptr;

	count = 0;
	va_start(ptr, format);
	while (*format != '\0')
	{
		if (*format == '%')
		{
			format++;
			count += ft_convert(ptr, *format);
		}
		else
		{
			count += ft_putchar(*format);
		}
		format++;
	}
	va_end(ptr);
	return (count);
}
