/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 14:45:17 by markhali          #+#    #+#             */
/*   Updated: 2025/12/30 22:18:54 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_node(t_node *ast, char *exit_str, t_env **env, t_gc *gc)
{
	int		exit_status;

	mark_builtins(ast);
	if (ast)
	{
		setup_signals_exec();
		exit_status = execute_node(ast, env, gc);
		if (exit_status == 131)
			write(2, "Quit (core dumped)\n", 19);
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

static int	prep_heredoc(t_node *ast, char *exit_str, t_env **env, t_gc *gc)
{
	int		heredoc_status;

	reset_signal();
	setup_signals_heredoc();
	heredoc_status = prepare_heredocs(ast, env, gc);
	setup_signals_interactive();
	if (heredoc_status == -1)
	{
		cleanup_heredocs(ast);
		set_exit_status(env, 130, exit_str);
		reset_signal();
		return (1);
	}
	return (0);
}

void	process_line(char *line, t_gc *gc, t_env **env)
{
	t_token	*tokens;
	t_node	*ast;
	char	exit_str[12];

	if (!line || line[0] == '\0')
		return ;
	add_history(line);
	if (check_syntax(line))
	{
		set_exit_status(env, 2, exit_str);
		return ;
	}
	tokens = tokenize_input(line, gc, env);
	ast = parse_pipeline(&tokens, env, gc);
	if (prep_heredoc(ast, exit_str, env, gc))
		return ;
	exec_node(ast, exit_str, env, gc);
}
