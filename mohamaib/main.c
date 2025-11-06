/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:00:00 by mohamaib          #+#    #+#             */
/*   Updated: 2025/11/05 23:59:59 by mohamaib         ###   ########.fr       */
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

void	process_line(char *line, t_gc *gc)
{
	t_token	*tokens;
	t_node	*ast;

	if (!line || line[0] == '\0')
		return ;
	add_history(line);
	tokens = tokenize_input(line, gc);
	print_token_list(tokens);
	ast = parse_pipeline(&tokens, gc);
	print_ast(ast);
}

int	main(int argc, char **argv, char **env)
{
	char	*line;
	t_gc	gc;

	(void)argc;
	(void)argv;
	(void)env;
	gc_init(&gc);
	while (1)
	{
		line = readline("minishell-> ");
		if (should_exit(line))
		{
			free(line);
			break ;
		}
		process_line(line, &gc);
		free(line);
		gc_free_all(&gc);
		gc_init(&gc);
	}
	gc_free_all(&gc);
	return (0);
}
