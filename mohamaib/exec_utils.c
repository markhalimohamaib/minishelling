/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:43:44 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/07 23:40:33 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	countwords(const char *s, char c)
{
	int	i;
	int	count;
	int	isword;

	if (!s)
		return (0);
	i = 0;
	count = 0;
	while (s[i])
	{
		isword = 0;
		if ((i == 0 && s[0] != c) || (s[i] != c && s[i - 1] == c))
			isword = 1;
		if (isword == 1)
			count++;
		i++;
	}
	return (count);
}

char	*allocate(const char *s, int start, int len, t_gc *gc)
{
	int		i;
	char	*ptr;

	i = 0;
	ptr = gc_malloc(sizeof (char) * (len + 1), gc);
	if (!ptr)
		return (NULL);
	while (s[start] && i < len)
	{
		ptr[i] = s[start + i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	**gc_ft_split(char const *s, char c, t_gc *gc)
{
	char	**big;
	int		start;
	int		end;
	int		i;
	int		j;

	i = 0;
	j = 0;
	big = gc_malloc(sizeof (char *) * (countwords(s, c) + 1), gc);
	if (!big)
		return (NULL);
	while (s[i])
	{
		if ((i == 0 && s[0] != c) || (s[i] != c && s[i - 1] == c))
			start = i;
		if ((s[i] != c && s[i + 1] == c) || (s[i] != c && s[i + 1] == '\0'))
		{
			end = i;
			if (j < countwords(s, c))
				big[j++] = allocate(s, start, end - start + 1, gc);
		}
		i++;
	}
	big[j] = NULL;
	return (big);
}

char	*ft_strjoin_plus(char const *s1, char const *s2, char const *s3, t_gc *gc)
{
	char	*new;
	int		i;
	int		j;
	int		w;

	if (!s1 && !s2)
		return (NULL);
	new = gc_malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)
				+ ft_strlen(s3) + 1), gc);
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	w = 0;
	while (s1 && s1[i] != '\0')
	{
		new[i] = s1[i];
		i++;
	}
	while (s2 && s2[j] != '\0')
		new[i++] = s2[j++];
	while (s3 && s3[w] != '\0')
		new[i++] = s3[w++];
	new[i] = '\0';
	return (new);
}

int	env_count(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->value)
			count++;
		env = env->next;
	}
	return (count);
}

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

// char	*get_path(t_env **env, t_gc *gc)
// {
// 	int		i;
// 	char	*path;

// 	i = 0;
// 	while ((*env))
// 	{
// 		if (ft_strncmp((*env)->key, "PATH", 5) == 0)
// 			path = gc_ft_strdup((*env)->value, gc);
// 		i++;
// 	}
// 	return (path);
// }

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
	char 	*path;
	char	**dirs;

	i = 0;
	envp = env_to_array((*env), gc);
	path = get_path(envp, gc);
	dirs = gc_ft_split(path, ':', gc);
	cmd  = gc_ft_strdup(node->cmd[0], gc);
	if (ft_strchr(cmd, '/'))
	{
		execve(cmd, node->cmd, envp);
		perror(cmd);
		exit(0);
	}
	while (dirs[i])
	{
		cmd = ft_strjoin_plus(dirs[i], "/", node->cmd[0], gc);
		if (access(cmd, X_OK) == 0)
		{
			// printf("execution complete1\n");
			execve(cmd, node->cmd, envp);
			exit(0);
		}
		i++;
	}
	printf("%s: command not found\n", node->cmd[0]);
	exit(127);
}

// void	child(t_node *node, t_env **env, t_gc *gc)
// {
// 	execute(node, env, gc);
// 	exit(127);
// }

int	exec_cmd(t_node *node, t_env **env, t_gc *gc)
{
	node->pid1 = fork();
	if (node->pid1 == 0)
		execute(node, env, gc);
	// printf("cmd not found2\n");
	waitpid(node->pid1, NULL, 0);
	return(1);
}
