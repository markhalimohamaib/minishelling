/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_tostr_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 00:35:24 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/24 00:35:41 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_num_length(int n)
{
	int	len;
	int	temp;

	len = 0;
	temp = n;
	if (temp < 0)
		temp = -temp;
	while (temp > 0)
	{
		temp /= 10;
		len++;
	}
	return (len);
}

void	fill_buffer_with_digits(char *buffer, int num, int pos)
{
	int	temp;

	temp = num;
	if (temp < 0)
		temp = -temp;
	while (temp > 0)
	{
		buffer[pos--] = (temp % 10) + '0';
		temp /= 10;
	}
}

char	*handle_zero_case(char *buffer)
{
	buffer[0] = '0';
	buffer[1] = '\0';
	return (buffer);
}
