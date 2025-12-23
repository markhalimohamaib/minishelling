/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 19:53:07 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/23 19:54:15 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_unclosed_quotes(char *str)
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

void	skip_quotes(char *str, int *i)
{
	char	quote;

	quote = str[*i];
	(*i)++;
	while (str[*i] && str[*i] != quote)
		(*i)++;
	if (str[*i])
		(*i)++;
}

int	handle_pipe(char *str, int *i, int *has_content)
{
	(*i)++;
	*has_content = 0;
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\t'))
		(*i)++;
	if (!str[*i] || str[*i] == '|')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	return (0);
}

int	check_pipe_helper(int *i, char *str)
{
	int	has_content;

	has_content = 0;
	while (str[(*i)])
	{
		if (str[(*i)] == '\'' || str[(*i)] == '\"')
		{
			skip_quotes(str, &(*i));
			has_content = 1;
		}
		else if (str[(*i)] == '|')
		{
			if (handle_pipe(str, &(*i), &has_content))
				return (1);
		}
		else if (str[(*i)] != ' ' && str[(*i)] != '\t')
		{
			has_content = 1;
			(*i)++;
		}
		else
			(*i)++;
	}
	return (0);
}

int	check_pipe_syntax(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] == '|')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	if (check_pipe_helper(&i, str))
		return (1);
	return (0);
}
