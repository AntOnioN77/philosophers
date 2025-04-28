/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 13:37:34 by antofern          #+#    #+#             */
/*   Updated: 2025/04/23 17:25:19 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

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

/*
#include <stdio.h>
#include "../headers/libft.h"

int main(void)
{
    const char *test1 = "12345";
    const char *test2 = "-67890";
    const char *test3 = "   +42";
    const char *test4 = NULL;

    printf("Test 1: \"%s\" -> %ld\n", test1, ft_atol(test1));
    printf("Test 2: \"%s\" -> %ld\n", test2, ft_atol(test2));
    printf("Test 3: \"%s\" -> %ld\n", test3, ft_atol(test3));
	printf("Test 4: \"%s\" -> %ld\n", test4, ft_atol(test4));

    return 0;
}
*/