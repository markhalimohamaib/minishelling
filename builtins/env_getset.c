/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_getset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:45:01 by markhali          #+#    #+#             */
/*   Updated: 2025/12/23 18:32:50 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static void	update_existing_env(t_env *tmp, const char *value)
{
	char	*new_value;

	if (value)
	{
		new_value = ft_strdup(value);
		if (!new_value)
			return ;
		free(tmp->value);
		tmp->value = new_value;
	}
	else
	{
		free(tmp->value);
		tmp->value = NULL;
	}
}

static t_env	*create_new_env(const char *key, const char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	if (!new->key)
	{
		free(new);
		return (NULL);
	}
	if (value)
	{
		new->value = ft_strdup(value);
		if (!new->value)
		{
			free(new->key);
			free(new);
			return (NULL);
		}
	}
	else
		new->value = NULL;
	return (new);
}

static void	set_new_env(t_env	**new, t_env **env)
{
	t_env	*last;

	if (!(*new))
		return ;
	if (!*env)
	{
		*env = (*new);
		(*new)->next = NULL;
	}
	else
	{
		last = *env;
		while (last->next)
			last = last->next;
		last->next = (*new);
		(*new)->next = NULL;
	}
}

void	set_env(t_env **env, const char *key, const char *value)
{
	t_env	*tmp;
	t_env	*new;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			update_existing_env(tmp, value);
			return ;
		}
		tmp = tmp->next;
	}
	new = create_new_env(key, value);
	set_new_env(&new, env);
}
