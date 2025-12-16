/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:43:44 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/16 18:49:47 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*env_join_kv(char *key, char *value, t_gc *gc)
{
	char	*str;
	size_t	klen;
	size_t	vlen;

	klen = ft_strlen(key);
	vlen = ft_strlen(value);
	str = gc_malloc(klen + vlen + 2, gc);
	if (!str)
		return (NULL);
	ft_memcpy(str, key, klen);
	str[klen] = '=';
	ft_memcpy(str + klen + 1, value, vlen);
	str[klen + 1 + vlen] = '\0';
	return (str);
}

int	env_fill_array(char **arr, t_env *env, t_gc *gc)
{
	int	i;

	i = 0;
	while (env)
	{
		if (env->value)
		{
			arr[i] = env_join_kv(env->key, env->value, gc);
			if (!arr[i])
				return (1);
			i++;
		}
		env = env->next;
	}
	arr[i] = NULL;
	return (0);
}

char	**env_to_array(t_env *env, t_gc *gc)
{
	char	**arr;
	int		size;

	size = env_count(env);
	arr = gc_malloc(sizeof(char *) * (size + 1), gc);
	if (!arr)
		return (NULL);
	if (env_fill_array(arr, env, gc))
		return (NULL);
	return (arr);
}

char	*get_path(char **env, t_gc *gc)
{
	int		i;
	char	*path;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			path = gc_ft_strdup(env[i] + 5, gc);
		i++;
	}
	return (path);
}

void	execute(t_node *node, t_env **env, t_gc *gc)
{
	int		i;
	char	*cmd;
	char	**envp;
	char	*path;
	char	**dirs;

	i = 0;
	envp = env_to_array((*env), gc);
	path = get_path(envp, gc);
	dirs = gc_ft_split(path, ':', gc);
	cmd = gc_ft_strdup(node->cmd[0], gc);
	setup_signals_child();
	if (ft_strchr(cmd, '/'))
	{
		execve(cmd, node->cmd, envp);
		perror(cmd);
		exit(127);
	}
	while (dirs[i])
	{
		cmd = ft_strjoin_plus(dirs[i], "/", node->cmd[0], gc);
		if (access(cmd, X_OK) == 0)
		{
			execve(cmd, node->cmd, envp);
			exit(127);
		}
		i++;
	}
	printf("%s: command not found\n", node->cmd[0]);
	exit(127);
}

int	get_exit_code_from_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		/* Handle signals: ctrl-C returns 130, ctrl-\ returns 131 */
		if (WTERMSIG(status) == SIGINT)
			return (130);
		else if (WTERMSIG(status) == SIGQUIT)
		{
			write(2, "Quit (core dumped)\n", 19);
			return (131);
		}
	}
	return (1);
}

int	exec_cmd(t_node *node, t_env **env, t_gc *gc)
{
	int	status;
	int	exit_code;

	node->pid1 = fork();
	if (node->pid1 == 0)
		execute(node, env, gc);
	waitpid(node->pid1, &status, 0);
	exit_code = get_exit_code_from_status(status);
	return (exit_code);
}
