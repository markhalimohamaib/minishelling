/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 23:05:36 by mohamaib          #+#    #+#             */
/*   Updated: 2025/11/05 23:31:25 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_cmd(t_node *node)
{
	int	pid;
	
	pid = fork();
}

void	execute_ast(t_node *root)
{
	if (root == NULL)
		return ;
	if (root->type == CMD_NODE)
		execute_cmd(root);
	else if (root->type == REDIR_NODE)
		execute_redirs(root);
	else if (root->type == PIPE_NODE)
		execute_pipe(root);
}
