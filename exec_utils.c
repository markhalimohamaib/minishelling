/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:43:44 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/25 19:59:26 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_command_type(char *cmd)
{
	struct stat	st;

	if (access(cmd, F_OK) != 0)
		return (127);
	if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		return (126);
	}
	if (access(cmd, X_OK) != 0)
	{
		perror(cmd);
		return (126);
	}
	return (0);
}

void	init_child(t_child_vars *child, t_node *node, t_env **env, t_gc *gc)
{
	child->i = 0;
	child->envp = env_to_array((*env), gc);
	child->path = gc_ft_strdup(get_path(child->envp, gc), gc);
	child->dirs = gc_ft_split(child->path, ':', gc);
	child->cmd = gc_ft_strdup(node->cmd[0], gc);
}

void	handle_executable(t_child_vars *child, t_node *node, int *check_result)
{
	(*check_result) = check_command_type(child->cmd);
	if ((*check_result) != 0)
		exit((*check_result));
	execve(child->cmd, node->cmd, child->envp);
	exit(127);
}

void	execute(t_node *node, t_env **env, t_gc *gc)
{
	int				check_result;
	t_child_vars	*child;

	child = gc_malloc(sizeof(t_child_vars), gc);
	init_child(child, node, env, gc);
	setup_signals_child();
	if (ft_strchr(child->cmd, '/'))
		handle_executable(child, node, &check_result);
	while (child->dirs[child->i])
	{
		child->cmd = ft_strjoin_plus(child->dirs[child->i], "/", node->cmd[0],
				gc);
		check_result = check_command_type(child->cmd);
		if (check_result == 0)
		{
			execve(child->cmd, node->cmd, child->envp);
			exit(127);
		}
		else if (check_result == 126)
			exit(126);
		child->i++;
	}
	ft_putstr_fd(node->cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(127);
}

int	exec_cmd(t_node *node, t_env **env, t_gc *gc)
{
	int	status;
	int	exit_code;

	status = 0;
	node->pid1 = fork();
	if (node->pid1 == 0)
		execute(node, env, gc);
	setup_signals_exec();
	waitpid(node->pid1, &status, 0);
	exit_code = get_exit_code_from_status(status);
	return (exit_code);
}
