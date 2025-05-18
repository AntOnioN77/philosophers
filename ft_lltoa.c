/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lltoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:09:18 by antofern          #+#    #+#             */
/*   Updated: 2025/05/18 22:17:32 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <limits.h>
static int	countchr(long long n)
{
	int			n_chars;
	long long	quotient;

	quotient = n;
	n_chars = 0;
	if (quotient == 0)
		return (1);
	else if (n < 0)
		n_chars++;
	while (quotient != 0)
	{
		quotient = quotient / 10;
		n_chars++;
	}
	return (n_chars);
}

static unsigned long long	calc_abs(const long long n, int *sign)
{
	unsigned long long	un;

	if (n < 0)
	{
		un = n * -1;
		*sign = -1;
	}
	else
	{
		un = (unsigned long long)n;
		*sign = 1;
	}
	return (un);
}

char	*ft_lltoa(long long n)
{
	int					n_chars;
	char				*result;
	int					sign;
	unsigned long long	un;

	n_chars = countchr(n);
	un = calc_abs(n, &sign);
	result = (char *)malloc(sizeof(char) * (n_chars + 1));
	if (result == NULL)
		return (NULL);
	if (un == 0)
		result[0] = '0';
	if (sign == -1)
		result[0] = '-';
	result[n_chars] = '\0';
	while (un != 0)
	{
		n_chars--;
		result[n_chars] = (un % 10) + '0';
		un = un / 10;
	}
	return (result);
}

/*
#include <stdio.h>
int	main(void)
{
    // Casos de prueba
    long long test_cases[] = {0, 1, 42, LLONG_MAX, LLONG_MIN, 10,
		100, 1000}; // Incluye el máximo valor de unsigned int
    size_t num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
    char *result;

    printf("Testing ft_uitoa:\n");
    for (size_t i = 0; i < num_cases; i++)
    {
        result = ft_itoa(test_cases[i]);
        if (result != NULL)
        {
            printf("Input: %lld -> Output: %s\n", test_cases[i], result);
            free(result); // Liberar la memoria asignada por ft_uitoa
        }
        else
        {
            printf("Input: %lld -> Output: NULL (Memory allocation failed)\n",
			test_cases[i]);
        }
    }

    return (0);
}
*/