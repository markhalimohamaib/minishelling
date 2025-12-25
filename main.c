/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:00:00 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/24 14:45:40 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_minishell(t_gc *gc, t_env **env, char **envp)
{
	gc_init(gc);
	*env = init_env(envp);
	if (!(*env))
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", 2);
		return (1);
	}
	init_shlvl(env);
	setup_signals_interactive();
	return (0);
}

static int	handle_input(char *line, t_gc *gc, t_env **env)
{
	process_line(line, gc, env);
	free(line);
	gc_free_all(gc);
	gc_init(gc);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_gc	gc;
	t_env	*env;
	char	*line;
	char	exit_str[12];

	(void)argc;
	(void)argv;
	if (init_minishell(&gc, &env, envp))
		return (1);
	set_exit_status(&env, 0, exit_str);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (handle_input(line, &gc, &env))
			break ;
	}
	gc_free_all(&gc);
	free_env(env);
	rl_clear_history();
	return (0);
}
