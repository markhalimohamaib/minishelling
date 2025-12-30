/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:29:31 by markhali          #+#    #+#             */
/*   Updated: 2025/12/30 20:26:27 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_builtin(t_node *node, t_env **env, t_gc *gc)
{
	if (node->builtin == BLT_ECHO)
		return (builtin_echo(node->cmd));
	else if (node->builtin == BLT_PWD)
		return (builtin_pwd());
	else if (node->builtin == BLT_CD)
		return (builtin_cd(node->cmd, env, gc));
	else if (node->builtin == BLT_ENV)
		return (builtin_env(*env));
	else if (node->builtin == BLT_EXPORT)
		return (builtin_export(env, node->cmd));
	else if (node->builtin == BLT_UNSET)
		return (builtin_unset(env, node->cmd));
	else if (node->builtin == BLT_EXIT)
		return (builtin_exit(node->cmd, env));
	return (0);
}

char	**handle_exp_spc(t_node *node, t_gc *gc)
{
	int		i;
	int		j;
	int		w;
	char	**val;
	char	**big;

	i = 0;
	w = 0;
	big = gc_malloc(sizeof(char *) * (get_full_cnt(node) + 1), gc);
	while (node->cmd[i])
	{
		if (ft_strchr(node->cmd[i], ' '))
		{
			val = gc_ft_split(node->cmd[i], ' ', gc);
			j = 0;
			while (val[j])
				big[w++] = gc_ft_strdup(val[j++], gc);
		}
		else
			big[w++] = gc_ft_strdup(node->cmd[i], gc);
		i++;
	}
	big[w] = NULL;
	return (big);
}

int	execute_command(t_node *node, t_env **env, t_gc *gc)
{
	if (!node || !node->cmd || !node->cmd[0])
		return (0);
	if (node->builtin == BLT_NONE)
	{
		node->cmd = handle_exp_spc(node, gc);
		mark_builtins(node);
	}
	if (node->builtin != BLT_NONE)
		return (execute_builtin(node, env, gc));
	if (node->builtin == BLT_NONE)
		return (exec_cmd(node, env, gc));
	printf("minishell: command not found: %s\n", node->cmd[0]);
	return (127);
}

static int	handle_redir_node(t_node *node, t_env **env, t_gc *gc)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
	{
		apply_redirs(node, env, gc);
		exit(execute_node(node->left, env, gc));
	}
	setup_signals_exec();
	waitpid(pid, &status, 0);
	return (status >> 8);
}

int	execute_node(t_node *node, t_env **env, t_gc *gc)
{
	if (!node)
		return (0);
	if (node->type == PIPE_NODE)
		return (execute_pipe(node, env, gc));
	if (node->type == REDIR_NODE)
		return (handle_redir_node(node, env, gc));
	if (node->type == CMD_NODE)
		return (execute_command(node, env, gc));
	return (0);
}
