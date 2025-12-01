/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:29:31 by markhali          #+#    #+#             */
/*   Updated: 2025/11/24 00:20:13 by mohamaib         ###   ########.fr       */
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

int	execute_command(t_node *node, t_env **env, t_gc *gc)
{
	if (!node || !node->cmd || !node->cmd[0])
		return (0);
	if (node->builtin != BLT_NONE)
		return (execute_builtin(node, env));
	if (node->builtin == BLT_NONE)
		return (exec_cmd(node, env, gc));
	write(2, "minishell: command not found: ", 30);
	write(2, node->cmd[0], ft_strlen(node->cmd[0]));
	write(2, "\n", 1);
	return (127);
}

int	execute_node(t_node *node, t_env **env, t_gc *gc)
{
	if (!node)
		return (0);
	if (node->type == PIPE_NODE)
		return (execute_pipe(node, env, gc));
	else if (node->type == REDIR_NODE)
	{
		if (node->redir_type == T_REDIR_IN)
			return (handle_redir_in(node, env, gc));
		else if (node->redir_type == T_REDIR_OUT)
			return (handle_redir_out(node, env, gc));
		else if (node->redir_type == T_REDIR_APPEND)
			return (handle_redir_append(node, env, gc));
		else if (node->redir_type == T_HEREDOC)
			return (handle_redir_heredoc(node, env, gc));
	}
	else if (node->type == CMD_NODE)
		return (execute_command(node, env, gc));
	return (0);
}
