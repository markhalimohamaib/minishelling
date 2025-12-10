/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:31:55 by markhali          #+#    #+#             */
/*   Updated: 2025/12/10 20:46:01 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_env	*new_env_node(const char *key, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (!node->key)
	{
		free(node);
		return (NULL);
	}
	if (value)
	{
		node->value = ft_strdup(value);
		if (!node->value)
		{
			free(node->key);
			free(node);
			return (NULL);
		}
	}
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

static void	add_env_node(t_env **head, t_env **tail, t_env *node)
{
	if (!*head)
		*head = node;
	else
		(*tail)->next = node;
	*tail = node;
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*tail;
	char	*eq;
	t_env	*node;
	int		i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (!eq)
		{
			i++;
			continue ;
		}
		*eq = '\0';
		node = new_env_node(envp[i], eq + 1);
		*eq = '=';
		if (!node)
			return (free_env(head), NULL);
		add_env_node(&head, &tail, node);
		i++;
	}
	return (head);
}
