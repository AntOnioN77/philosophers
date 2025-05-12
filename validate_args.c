/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:40:31 by antofern          #+#    #+#             */
/*   Updated: 2025/05/11 22:05:32 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int validate_args(int argc, char **argv)
{
    int i;
    if (argc < 5 || argc > 6)
    {
        printf("Error: Invalid number of arguments.\n");
        return (1);
    }
    if(!is_in_bounds_uint(argv[1]))
    {
        printf("Error: 1º Argument is not a valid number.\n");
        printf("MAX:4294967295\nMIN:0\nDo not use letters, spaces, commas or dots.\n");
        return (1);
    }
    i = 2;
    while (i < 5)
    {
        if (!valid_amount_ms(argv[i]))
        {
            printf("Error: %dº Argument is not a valid number.\n", i);
            printf("MAX:4294967 (useconds_t MAX/1000)\nMIN:0\nDo not use letters, spaces, commas or dots.\n");
            return (1);
        }
        i++;
    }
    if(argc == 6 && !is_in_bounds_uint(argv[5]))
    {
        printf("Error: 1º Argument is not a valid number.\n");
        printf("MAX:4294967295\nMIN:0\nDo not use letters, spaces, commas or dots.\n");
        return (1);
    }
    return (0);
}
