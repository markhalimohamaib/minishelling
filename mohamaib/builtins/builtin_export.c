/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 15:55:04 by markhali          #+#    #+#             */
/*   Updated: 2025/11/12 10:34:35 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!(str[0] == '_' || (str[0] >= 'a' && str[0] <= 'z')
			|| (str[0] >= 'A' && str[0] <= 'Z')))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!(str[i] == '_' || (str[i] >= 'a' && str[i] <= 'z')
				|| (str[i] >= 'A' && str[i] <= 'Z')
				|| (str[i] >= '0' && str[i] <= '9')))
			return (0);
		i++;
	}
	return (1);
}

static void	print_export(t_env *env)
{
	while (env)
	{
		if (env->value)
			printf("declare -x %s=\"%s\"\n", env->key, env->value);
		else
			printf("declare -x %s\n", env->key);
		env = env->next;
	}
}

static void	handle_export_with_value(t_env **env, char *arg)
{
	char	*eq;
	char	*key;
	char	*value;

	eq = ft_strchr(arg, '=');
	*eq = '\0';
	key = arg;
	value = eq + 1;
	set_env(env, key, value);
	*eq = '=';
}

static int	process_arg(t_env **env, char *arg)
{
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	if (ft_strchr(arg, '='))
		handle_export_with_value(env, arg);
	else if (!get_env(*env, arg))
		set_env(env, arg, NULL);
	return (0);
}

int	builtin_export(t_env **env, char **args)
{
	int	i;
	int	ret;

	if (!args[1])
	{
		print_export(*env);
		return (0);
	}
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (process_arg(env, args[i]))
			ret = 1;
		i++;
	}
	return (ret);
}
