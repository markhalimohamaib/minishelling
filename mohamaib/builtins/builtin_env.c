/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 15:38:11 by markhali          #+#    #+#             */
/*   Updated: 2025/12/17 17:15:07 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_env(t_env *env)
{
	while (env)
	{
		if (env->value && env->key[0] != '?')
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}
