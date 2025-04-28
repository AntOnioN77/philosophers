/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:40:31 by antofern          #+#    #+#             */
/*   Updated: 2025/04/23 16:00:11 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int validate_args(int argc, char **argv)
{
    int i;
    if (argc < 4 || argc > 6)
    {
        printf("Error: Invalid number of arguments.\n");
        return (1);
    }
    i = 1;
    while (i < argc)
    {
        if (!ft_is_in_bounds_int(argv[i]))
        {
            printf("Error: Argument %d is not a valid useconds_t number.\n", i);
            printf("MAX:4294967295\nMIN:0\nDo not use letters, spaces, commas or dots.\n");
            return (1);
        }
        i++;
    }
    return (0);
}
