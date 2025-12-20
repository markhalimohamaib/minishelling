/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 20:29:10 by markhali          #+#    #+#             */
/*   Updated: 2025/12/20 21:43:58 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_exit_status(t_env **env, int status, char *buffer)
{
	int		i;
	int		temp;
	int		len;

	if (status == 0)
	{
		buffer[0] = '0';
		buffer[1] = '\0';
	}
	else
	{
		temp = status;
		len = 0;
		while (temp > 0)
		{
			temp /= 10;
			len++;
		}
		buffer[len] = '\0';
		i = len - 1;
		while (status > 0)
		{
			buffer[i--] = (status % 10) + '0';
			status /= 10;
		}
	}
	set_env(env, "?", buffer);
}

char	*get_exit_status(t_env *env)
{
	return (get_env(env, "?"));
}

int	get_exit_code_from_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
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
