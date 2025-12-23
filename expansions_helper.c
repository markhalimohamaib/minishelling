/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 23:04:03 by markhali          #+#    #+#             */
/*   Updated: 2025/12/20 23:21:40 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_dollars(t_segment seg)
{
	int	i;
	int	doll_count;

	i = 0;
	doll_count = 0;
	while (seg.str[i])
	{
		if (seg.str[i] == '$' && seg.str[i + 1] == '$')
		{
			doll_count++;
			i++;
		}
		else if (seg.str[i] == '$' && seg.str[i + 1] != '$')
			doll_count++;
		i++;
	}
	return (doll_count);
}

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

char	*gc_ft_itoa(int n, t_gc *gc)
{
	int		len;
	char	*str;
	long	nb;

	nb = n;
	len = count_digits(n);
	str = gc_malloc((len + 1), gc);
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

char	*replace_val_in_env(char *val, t_env **env, t_gc *gc)
{
	t_env	*tmp;
	
	if (ft_strcmp(val, "?") == 0)
	{
		char	*exit_status;
		exit_status = get_env(*env, "?");
		if (exit_status)
			return (gc_ft_strdup(exit_status, gc));
		return (gc_ft_strdup("0", gc));
	}
	if (ft_strcmp(val, "$") == 0)
	{
		char	*pid;
		pid = gc_ft_itoa(getpid(), gc);
		return (gc_ft_strdup(pid, gc));
	}
	tmp = (*env);
	while (tmp)
	{
		if (tmp->value && !(ft_strcmp(tmp->key, val)))
		{
			return (gc_ft_strdup(tmp->value, gc));
		}
		tmp = tmp->next;
	}
	return (NULL);
}

int	get_lead_val_size(t_segment seg)
{
	int	size;
	int	i;

	i = 0;
	size = 0;
	while (seg.str[i] != '$')
	{
		i++;
		size++;
	}
	return (size + 1);
}

int	get_expandable_size(t_segment seg)
{
	int	size;
	int	i;

	i = 0;
	size = 0;
	while (seg.str[i])
	{
		if (seg.str[i] == '$')
		{
			i++;
			while (seg.str[i] && seg.str[i] != ' ' && seg.str[i] != '|'
				&& seg.str[i] != '>' && seg.str[i] != '<')
			{
				size++;
				i++;
			}
			break ;
		}
		i++;
	}
	return (size + 1);
}
