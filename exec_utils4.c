/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 21:38:07 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/24 00:04:22 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
