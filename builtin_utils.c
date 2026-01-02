/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 16:24:05 by markhali          #+#    #+#             */
/*   Updated: 2026/01/02 00:45:56 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

t_builtin_type	get_builtin_type(char *cmd)
{
	if (!cmd)
		return (BLT_NONE);
	if (ft_strcmp(cmd, "echo") == 0)
		return (BLT_ECHO);
	if (ft_strcmp(cmd, "cd") == 0)
		return (BLT_CD);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (BLT_PWD);
	if (ft_strcmp(cmd, "export") == 0)
		return (BLT_EXPORT);
	if (ft_strcmp(cmd, "unset") == 0)
		return (BLT_UNSET);
	if (ft_strcmp(cmd, "env") == 0)
		return (BLT_ENV);
	if (ft_strcmp(cmd, "exit") == 0)
		return (BLT_EXIT);
	return (BLT_NONE);
}

void	mark_builtins(t_node *node)
{
	if (!node)
		return ;
	if (node->type == CMD_NODE && node->cmd && node->cmd[0])
		node->builtin = get_builtin_type(node->cmd[0]);
	if (node->left)
		mark_builtins(node->left);
	if (node->right)
		mark_builtins(node->right);
}
