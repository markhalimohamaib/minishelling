/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 21:23:11 by markhali          #+#    #+#             */
/*   Updated: 2025/12/20 21:25:32 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_unclosed_quotes(char *str)
{
	int		i;
	char	quote;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			quote = str[i];
			i++;
			while (str[i] && str[i] != quote)
				i++;
			if (!str[i])
			{
				ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

static int	check_pipe_syntax(char *str)
{
	int	i;
	int	has_content;

	i = 0;
	has_content = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] == '|')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			char quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			if (str[i])
				i++;
			has_content = 1;
		}
		else if (str[i] == '|')
		{
			i++;
			has_content = 0;
			while (str[i] && (str[i] == ' ' || str[i] == '\t'))
				i++;
			if (!str[i] || str[i] == '|')
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
				return (1);
			}
		}
		else if (str[i] != ' ' && str[i] != '\t')
		{
			has_content = 1;
			i++;
		}
		else
			i++;
	}
	return (0);
}

static int	check_redir_syntax(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			char quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			if (str[i])
				i++;
		}
		else if (str[i] == '<' || str[i] == '>')
		{
			char redir = str[i];
			i++;
			if (str[i] == redir)
				i++;
			while (str[i] && (str[i] == ' ' || str[i] == '\t'))
				i++;
			if (!str[i] || str[i] == '|' || str[i] == '<' || str[i] == '>')
			{
				if (!str[i])
					ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				else if (str[i] == '|')
					ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
				else if (str[i] == '<')
					ft_putstr_fd("minishell: syntax error near unexpected token `<'\n", 2);
				else if (str[i] == '>')
					ft_putstr_fd("minishell: syntax error near unexpected token `>'\n", 2);
				return (1);
			}
		}
		else
			i++;
	}
	return (0);
}

int	check_syntax(char *line)
{
	if (!line || !line[0])
		return (0);
	if (check_unclosed_quotes(line))
		return (1);
	if (check_pipe_syntax(line))
		return (1);
	if (check_redir_syntax(line))
		return (1);
	return (0);
}
