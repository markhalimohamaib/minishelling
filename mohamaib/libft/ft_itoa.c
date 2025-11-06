/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 19:19:44 by mohamaib          #+#    #+#             */
/*   Updated: 2025/06/04 00:53:45 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	count_digits(int n)
{
	int	digits;

	digits = 0;
	if (n <= 0)
		digits = 1;
	while (n)
	{
		n /= 10;
		digits++;
	}
	return (digits);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*str;
	long	nb;

	nb = n;
	len = count_digits(n);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len--] = '\0';
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	if (nb == 0)
		str[0] = '0';
	while (nb > 0)
	{
		str[len--] = (nb % 10) + '0';
		nb /= 10;
	}
	return (str);
}
// int	countdig(int c)
// {
// 	int	i;

// 	if (c == 0)
// 		return (1);
// 	if (c < 0)
// 		c = -c;
// 	i = 0;
// 	while (c != 0)
// 	{
// 		c /= 10;
// 		i++;
// 	}
// 	return (i);
// }

// char	*reverse(char *s, long c)
// {
// 	int		i;
// 	int		j;
// 	char	temp;

// 	if (!s)
// 		return (NULL);
// 	if (c < 0)
// 		i = 1;
// 	else
// 		i = 0;

// 	j = ft_strlen(s);
// 	while (i < j)
// 	{
// 		temp = s[i];
// 		s[i] = s[j - 1];
// 		s[j - 1] = temp;
// 		i++;
// 		j--;
// 	}
// 	return (s);
// }

// char	*tochar(long c)
// {
// 	char	*str;
// 	int		i;
// 	long	j;

// 	i = 0;
// 	if (c < 0)
// 		j = -c;
// 	else
// 		j = c;

// 	str = (char *)malloc(sizeof (char) * (countdig(c) + 1));
// 	if (!str)
// 		return (NULL);
// 	if (c < 0)
// 		str[i++] = '-';
// 	if (j == 0)
// 		str[i++] = '0';
// 	while (j != 0)
// 	{
// 		str[i++] = (j % 10) + '0';
// 		j /= 10;
// 	}
// 	str[i] = '\0';
// 	return (str);
// }

// char	*ft_itoa(int c)
// {
// 	char	*str;

// 	if (c == 0)
// 	{
// 		str = (char *)malloc(2);
// 		if (!str)
// 			return (NULL);
// 		str[0] = '0';
// 		str[1] = '\0';
// 		return (str);
// 	}
// 	if (c == -2147483648)
// 	{
// 		str = tochar((long)c);
// 		str = reverse(str, (long)c);
// 		return (str);
// 	}
// 	str = tochar(c);
// 	str = reverse(str, c);
// 	return (str);
// }
// #include <stdio.h>
// int main (void)
// {
// 	char	*str;
// 	str =  ft_itoa(-2147483648);
// 	printf("%s", str);
// 	free(str);
// }