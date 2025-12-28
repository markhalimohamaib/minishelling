/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:19:34 by markhali          #+#    #+#             */
/*   Updated: 2025/12/27 19:57:27 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_atoi_shlvl(const char *str)
{
	int		i;
	int		sign;
	long	result;

	if (!str)
		return (0);
	i = skip_whitespace(str);
	sign = parse_sign(str, &i);
	if (!ft_isdigit(str[i]))
		return (0);
	result = parse_digits(str, &i, sign);
	if (result == 0 && is_overflow(result, sign))
		return (0);
	return ((int)(result * sign));
}

static int	is_valid_shlvl(const char *str)
{
	int	i;

	i = 0;
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

char	*int_to_str(int n, char *buffer)
{
	int	len;
	int	is_neg;

	if (n == 0)
		return (handle_zero_case(buffer));
	is_neg = 0;
	if (n < 0)
		is_neg = 1;
	len = get_num_length(n);
	buffer[len + is_neg] = '\0';
	fill_buffer_with_digits(buffer, n, len + is_neg - 1);
	if (is_neg)
		buffer[0] = '-';
	return (buffer);
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
			shlvl = 0;
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
