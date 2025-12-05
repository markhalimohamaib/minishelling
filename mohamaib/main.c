/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:00:00 by mohamaib          #+#    #+#             */
/*   Updated: 2025/11/30 21:50:31 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	should_exit(char *line)
{
	if (!line)
		return (1);
	if (ft_strncmp(line, "exit", 4) == 0 && (line[4] == '\0'
			|| line[4] == ' ' || line[4] == '\t'))
		return (1);
	return (0);
}

void	process_line(char *line, t_gc *gc, t_env **env)
{
	t_token	*tokens;
	t_node	*ast;

	if (!line || line[0] == '\0')
		return ;
	add_history(line);
	tokens = tokenize_input(line, gc);
	print_token_list(tokens);
	ast = parse_pipeline(&tokens, gc);
	mark_builtins(ast);
	print_ast(ast);
	if (ast)
		execute_node(ast, env, gc);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_gc	gc;
	t_env	*env;

	(void)argc;
	(void)argv;
	gc_init(&gc);
	env = init_env(envp);
	if (!env)
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", 2);
		return (1);
	}
	while (1)
	{
		line = readline("minishell$ ");
		if (should_exit(line))
		{
			free(line);
			break ;
		}
		process_line(line, &gc, &env);
		free(line);
		gc_free_all(&gc);
		gc_init(&gc);
	}
	gc_free_all(&gc);
	free_env(env);
	return (0);
}
