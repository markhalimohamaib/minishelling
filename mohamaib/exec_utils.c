/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:43:44 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/17 17:49:00 by markhali         ###   ########.fr       */
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

void	execute(t_node *node, t_env **env, t_gc *gc)
{
	int		i;
	char	*cmd;
	char	**envp;
	char	*path;
	char	**dirs;
	int		check_result;

	i = 0;
	envp = env_to_array((*env), gc);
	path = get_path(envp, gc);
	dirs = gc_ft_split(path, ':', gc);
	cmd = gc_ft_strdup(node->cmd[0], gc);
	setup_signals_child();
	if (ft_strchr(cmd, '/'))
	{
		check_result = check_command_type(cmd);
		if (check_result != 0)
			exit(check_result);
		execve(cmd, node->cmd, envp);
		perror(cmd);
		exit(127);
	}
	while (dirs[i])
	{
		cmd = ft_strjoin_plus(dirs[i], "/", node->cmd[0], gc);
		check_result = check_command_type(cmd);
		if (check_result == 0)
		{
			execve(cmd, node->cmd, envp);
			exit(127);
		}
		else if (check_result == 126)
			exit(126);
		i++;
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
	waitpid(node->pid1, &status, 0);
	exit_code = get_exit_code_from_status(status);
	return (exit_code);
}
