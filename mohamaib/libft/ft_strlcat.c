/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:42:57 by mohamaib          #+#    #+#             */
/*   Updated: 2025/06/03 23:38:27 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	dest_len;
	unsigned int	src_len;

	j = 0;
	dest_len = 0;
	src_len = 0;
	while (src [src_len] != '\0')
		src_len++;
	while (dest[dest_len] != '\0' && dest_len < size)
		dest_len++;
	if (size <= dest_len)
		return (size + src_len);
	i = dest_len + src_len;
	while (src[j] != '\0' && (dest_len + j) < size - 1)
	{
		dest[dest_len + j] = src[j];
		j++;
	}
	dest[dest_len + j] = '\0';
	return (i);
}
