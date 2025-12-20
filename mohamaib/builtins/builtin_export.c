/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 15:55:04 by markhali          #+#    #+#             */
/*   Updated: 2025/12/20 22:13:27 by markhali         ###   ########.fr       */
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

static t_env	*copy_env_node(t_env *node)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(node->key);
	if (!new->key)
	{
		free(new);
		return (NULL);
	}
	if (node->value)
	{
		new->value = ft_strdup(node->value);
		if (!new->value)
		{
			free(new->key);
			free(new);
			return (NULL);
		}
	}
	else
		new->value = NULL;
	new->next = NULL;
	return (new);
}

static t_env	*sort_env_list(t_env *env)
{
	t_env	*sorted;
	t_env	*current;
	t_env	*new_node;
	t_env	*tmp;
	t_env	*prev;

	sorted = NULL;
	current = env;
	while (current)
	{
		new_node = copy_env_node(current);
		if (!new_node)
		{
			free_env(sorted);
			return (NULL);
		}
		if (!sorted || ft_strcmp(new_node->key, sorted->key) < 0)
		{
			new_node->next = sorted;
			sorted = new_node;
		}
		else
		{
			tmp = sorted;
			prev = NULL;
			while (tmp && ft_strcmp(new_node->key, tmp->key) > 0)
			{
				prev = tmp;
				tmp = tmp->next;
			}
			new_node->next = tmp;
			if (prev)
				prev->next = new_node;
		}
		current = current->next;
	}
	return (sorted);
}

static void	print_export(t_env *env)
{
	t_env	*sorted;
	t_env	*tmp;

	sorted = sort_env_list(env);
	if (!sorted)
		return ;
	
	tmp = sorted;
	while (tmp)
	{
		if (tmp->value)
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		else
			printf("declare -x %s\n", tmp->key);
		tmp = tmp->next;
	}
	free_env(sorted);
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
