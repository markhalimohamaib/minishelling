/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:19:34 by markhali          #+#    #+#             */
/*   Updated: 2025/12/18 19:18:39 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_atoi_shlvl(const char *str)
{
	long	result = 0;
	int		sign = 1;
	int		i = 0;

	if (!str)
		return (0);
	while (str[i] && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!ft_isdigit(str[i]))
		return (0);
	while (ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		if (result * sign > 2147483647 || result * sign < -2147483648)
			return (0);
		i++;
	}
	return ((int)(result * sign));
}

static int	is_valid_shlvl(const char *str)
{
	int i = 0;

	if (!str || !str[0])
		return (0);
	while (str[i] && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static char	*int_to_str(int n, char *buffer)
{
	int	i = 0;
	int temp = n;
	int	is_neg = 0;
	int len = 0;
	int tmp = temp;

	if (n == 0)
	{
		buffer[0] = '0';
		buffer[1] = '\0';
		return buffer;
	}
	if (n < 0)
	{
		is_neg = 1;
		temp = -temp;
	}
	while (tmp > 0)
	{
		tmp /= 10;
		len++;
	}
	buffer[len + is_neg] = '\0';
	i = len + is_neg - 1;

	while (temp > 0)
	{
		buffer[i--] = (temp % 10) + '0';
		temp /= 10;
	}
	if (is_neg)
		buffer[0] = '-';

	return buffer;
}

void	init_shlvl(t_env **env)
{
	char	*shlvl_str;
	int		shlvl;
	char	buffer[12];

	shlvl_str = get_env(*env, "SHLVL");
	if (!shlvl_str || !is_valid_shlvl(shlvl_str))
		shlvl = 1;
	else
	{
		shlvl = ft_atoi_shlvl(shlvl_str);
		if (shlvl < 0)
			shlvl = 1;
		else if (shlvl >= 999)
		{
			ft_putstr_fd("minishell: warning: shell level (", 2);
			ft_putstr_fd(shlvl_str, 2);
			ft_putstr_fd(") too high, resetting to 1\n", 2);
			shlvl = 1;
		}
		else
			shlvl++;
	}
	int_to_str(shlvl, buffer);
	set_env(env, "SHLVL", buffer);
}
