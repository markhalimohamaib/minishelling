/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 02:07:23 by mohamaib          #+#    #+#             */
/*   Updated: 2025/06/03 23:30:09 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	i;

	if (n == 0)
	{
		write (fd, "0", 1);
		return ;
	}
	if (n == -2147483648)
	{
		write (fd, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		write (fd, "-", 1);
		n *= -1;
	}
	i = '0';
	if (n >= 10)
		ft_putnbr_fd(n / 10, fd);
	i = (n % 10) + '0';
	write (fd, &i, 1);
}
// int main (void)
// {
// 	ft_putnbr_fd(-9000876, 1);
// }