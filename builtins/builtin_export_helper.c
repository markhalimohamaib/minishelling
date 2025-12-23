/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 18:52:44 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/23 19:10:05 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*copy_env_node(t_env *node)
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

void	sort_env_helper(t_env	**sorted, t_env	**new_node)
{
	t_env	*tmp;
	t_env	*prev;

	if (!*sorted || ft_strcmp((*new_node)->key, (*sorted)->key) < 0)
	{
		(*new_node)->next = (*sorted);
		(*sorted) = (*new_node);
	}
	else
	{
		tmp = (*sorted);
		prev = NULL;
		while (tmp && ft_strcmp((*new_node)->key, tmp->key) > 0)
		{
			prev = tmp;
			tmp = tmp->next;
		}
		(*new_node)->next = tmp;
		if (prev)
			prev->next = (*new_node);
	}
}

t_env	*sort_env_list(t_env *env)
{
	t_env	*sorted;
	t_env	*current;
	t_env	*new_node;

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
		sort_env_helper(&sorted, &new_node);
		current = current->next;
	}
	return (sorted);
}

void	print_export(t_env *env)
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

void	handle_export_with_value(t_env **env, char *arg)
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
