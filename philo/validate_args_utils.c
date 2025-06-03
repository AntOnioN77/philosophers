/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_args_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:07:40 by antofern          #+#    #+#             */
/*   Updated: 2025/05/20 09:44:10 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int		ft_isdigit(int c);
long	ft_atol(const char *str);

t_bool	valid_amount_ms(char *num)
{
	long	nbr;
	size_t	i;
	t_bool	sign;

	i = 0;
	sign = FALSE;
	if (num[0] == '-')
		return (FALSE);
	else if (num[0] == '+')
	{
		sign = TRUE;
		i++;
	}
	while (ft_isdigit(num[i]))
		i++;
	if ((i - sign) < 1 || i != ft_strlen(num))
		return (FALSE);
	if (ft_strlen(num) > (size_t)(7 + sign))
		return (FALSE);
	nbr = ft_atol(num) * 1000;
	if (nbr > UINT_MAX || nbr < 0)
		return (FALSE);
	return (TRUE);
}

t_bool	is_in_bounds(char *num)
{
	long	nbr;
	size_t	i;
	t_bool	sign;

	i = 0;
	sign = FALSE;
	if (num[0] == '-')
		return (FALSE);
	else if (num[0] == '+')
	{
		sign = TRUE;
		i++;
	}
	while (ft_isdigit(num[i]))
		i++;
	if ((i - sign) < 1 || i != ft_strlen(num))
		return (FALSE);
	if (ft_strlen(num) > (size_t)(10 + sign))
		return (FALSE);
	nbr = ft_atol(num);
	if (nbr > INT_MAX || nbr < 0)
		return (FALSE);
	return (TRUE);
}

int	ft_isdigit(int c)
{
	if (c > '/' && c < ':')
	{
		return (1);
	}
	return (0);
}

long	ft_atol(const char *str)
{
	long		total;
	long		sign;

	sign = 1;
	total = 0;
	if (!str)
		return (0);
	while (*str == '\n' || *str == '\t' || *str == '\r'
		|| *str == '\v' || *str == '\f' || *str == ' ')
		str++;
	if (*str == '-' && (ft_isdigit(str[1]) != 0))
	{
		sign = sign * (-1);
		str++;
	}
	if (*str == '+')
		str++;
	while (ft_isdigit(*str) != 0)
	{
		total = (total * 10) + (*str - '0');
		str++;
	}
	return (total * sign);
}
