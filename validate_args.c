/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:40:31 by antofern          #+#    #+#             */
/*   Updated: 2025/05/19 15:05:59 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
static int	validate_philosopher_count(char *arg)
{
	long	n_philos;

	n_philos = ft_atol(arg);
	if (!is_in_bounds(arg) || n_philos > 200 || n_philos <= 0)
	{
		printf("Error: 1º Argument is not a valid number.\n");
		printf("MAX:200\nMIN:1\nDo not use letters, spaces, commas or dots.\n");
		return (1);
	}
	return (0);
}

static int	validate_time_args(char **argv)
{
	int	i;

	i = 2;
	while (i < 5)
	{
		if (!valid_amount_ms(argv[i]))
		{
			printf("Error: %dº Argument is not a valid number.\n", i);
			printf("MAX:4294967 (useconds_t MAX/1000)\nMIN:0\nDo not use letters,\
				spaces, commas or dots.\n");
			return (1);
		}
		i++;
	}
	return (0);
}

static int	validate_optional_arg(int argc, char **argv)
{
	if (argc == 6 && !is_in_bounds(argv[5]))
	{
		printf("Error: 5º Argument is not a valid number.\nMAX:2147483647\n");
		printf("MIN:0\nDo not use letters, spaces, commas or dots.\n");
		return (1);
	}
	return (0);
}

int	validate_args(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		printf("Error: Invalid number of arguments.\n");
		return (1);
	}
	if (validate_philosopher_count(argv[1]))
		return (1);
	if (validate_time_args(argv))
		return (1);
	if (validate_optional_arg(argc, argv))
		return (1);
	return (0);
}
