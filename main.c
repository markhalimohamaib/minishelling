/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:00:00 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/20 22:57:58 by mohamaib         ###   ########.fr       */
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
	char	exit_str[12];

	if (!line || line[0] == '\0')
		return ;
	add_history(line);
	if (check_syntax(line))
	{
		set_exit_status(env, 2, exit_str);
		return ;
	}
	tokens = tokenize_input(line, gc);
	// print_token_list(tokens);
	ast = parse_pipeline(&tokens, env, gc);
	reset_signal();
	setup_signals_heredoc();
	heredoc_status = prepare_heredocs(ast, env, gc);
	setup_signals_interactive();
	if (heredoc_status == -1)
	{
		cleanup_heredocs(ast);
		set_exit_status(env, 130, exit_str);
		reset_signal();
		return ;
	}
	mark_builtins(ast);
	// print_ast(ast);
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
		set_exit_status(env, exit_status, exit_str);
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
