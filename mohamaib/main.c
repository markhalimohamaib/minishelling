/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:00:00 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/16 19:55:42 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	should_exit(char *line)
{
	if (!line)
		return (1);
	if (ft_strncmp(line, "exit", 4) == 0 && (line[4] == '\0' || line[4] == ' '
			|| line[4] == '\t'))
		return (1);
	return (0);
}

void	process_line(char *line, t_gc *gc, t_env **env)
{
	t_token	*tokens;
	t_node	*ast;
	int		exit_status;
	int		heredoc_status;

	if (!line || line[0] == '\0')
		return ;
	add_history(line);
	tokens = tokenize_input(line, gc);
	print_token_list(tokens);
	ast = parse_pipeline(&tokens, env, gc);
	reset_signal();
	setup_signals_heredoc();
	heredoc_status = prepare_heredocs(ast, env, gc);
	setup_signals_interactive();
	if (heredoc_status == -1)
	{
		cleanup_heredocs(ast);
		reset_signal();
		return ;
	}
	mark_builtins(ast);
	print_ast(ast);
	if (ast)
	{
		setup_signals_exec();
		exit_status = execute_node(ast, env, gc);
		setup_signals_interactive();
		if (g_signal)
		{
			exit_status = get_signal_exit_status();
			reset_signal();
		}
	}
	cleanup_heredocs(ast);
}

static int	init_minishell(t_gc *gc, t_env **env, char **envp)
{
	gc_init(gc);
	*env = init_env(envp);
	if (!(*env))
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", 2);
		return (1);
	}
	setup_signals_interactive();
	return (0);
}

static int	handle_input(char *line, t_gc *gc, t_env **env)
{
	if (should_exit(line))
	{
		free(line);
		return (1);
	}
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

	(void)argc;
	(void)argv;
	if (init_minishell(&gc, &env, envp))
		return (1);
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
