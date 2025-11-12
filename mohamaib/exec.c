/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:29:31 by markhali          #+#    #+#             */
/*   Updated: 2025/11/12 13:41:36 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_builtin(t_node *node, t_env **env)
{
	if (node->builtin == BLT_ECHO)
		return (builtin_echo(node->cmd));
	else if (node->builtin == BLT_PWD)
		return (builtin_pwd());
	else if (node->builtin == BLT_CD)
		return (builtin_cd(node->cmd, env));
	else if (node->builtin == BLT_ENV)
		return (builtin_env(*env));
	else if (node->builtin == BLT_EXPORT)
		return (builtin_export(env, node->cmd));
	else if (node->builtin == BLT_UNSET)
		return (builtin_unset(env, node->cmd));
	else if (node->builtin == BLT_EXIT)
		return (builtin_exit(node->cmd));
	return (0);
}

int	execute_command(t_node *node, t_env **env)
{
	if (!node || !node->cmd || !node->cmd[0])
		return (0);
	// For now, only execute built-ins
	if (node->builtin != BLT_NONE)
		return (execute_builtin(node, env));
	// External commands will be implemented later
	printf("minishell: command not found: %s\n", node->cmd[0]);
	return (127);
}

int	execute_node(t_node *node, t_env **env)
{
	if (!node)
		return (0);
	if (node->type == CMD_NODE)
		return (execute_command(node, env));
	else if (node->type == REDIR_NODE)
	{
		// For now, just execute the left node (command)
		// Redirections will be implemented later
		printf("Note: Redirection detected but not yet implemented\n");
		if (node->left)
			return (execute_node(node->left, env));
	}
	else if (node->type == PIPE_NODE)
	{
		// Pipes will be implemented later
		printf("Note: Pipeline detected but not yet implemented\n");
		// For now, just execute left side
		if (node->left)
			execute_node(node->left, env);
		if (node->right)
			execute_node(node->right, env);
	}
	return (0);
}
