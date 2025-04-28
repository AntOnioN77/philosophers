/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_in_buonds_uint.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:02:58 by antofern          #+#    #+#             */
/*   Updated: 2025/04/23 15:48:45 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../headers/libft.h"
#include <limits.h>

/*Receives a pointer to the first character of a string <char *digits>,
with or within an unique simbol (+-).
Returns TRUE if the number can be represented as an unsigned int type.*/
t_bool	ft_is_in_bounds_uint(char *num)
{
	long	nbr;
	size_t	i;
	t_bool	sign;

	i = 0;
	sign = FALSE;
	if (num[0] == '-')
        return (FALSE);
    else if(num[0] == '+')
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
	if (nbr > UINT_MAX || nbr < 0)
		return (FALSE);
	return (TRUE);
}
/*
#include <stdio.h>
//cc -Wall -Wextra -Werror is_in_buonds_uint.c -I../headers/libft.h -L.. -lft
int main()
{
    char *nums_in[8] = {"0", "4294967295", "+4294967295", "+1", "1234567890", "18901", "100", "3"};
    char *nums_out[13] = {"4294967296","+4294967296", "-4294967295", "-4294967296", "-1", "-100", "-3", "12345678901", "123abc", "1.5", "1.0", "asdf10", "  10"};
    int i = 0;
    printf("Testing ft_is_in_bounds_uint\n");
    printf("Testing valid numbers:\n");
    while (i<=7)
    {
        if (!ft_is_in_bounds_uint(nums_in[i]))
            printf("error: %s\n", nums_in[i]);
        else
            printf("ok: %s\n", nums_in[i]);
        i++;
    }
    i = 0;
    printf("Testing invalid numbers:\n");
    while (i<=12)
    {
        if (ft_is_in_bounds_uint(nums_out[i]))
            printf("error: %s\n", nums_out[i]);
        else
            printf("ok: %s\n", nums_out[i]);
        i++;
    }
    return 0;
}
*/