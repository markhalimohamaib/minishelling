/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:00:00 by mohamaib          #+#    #+#             */
/*   Updated: 2025/11/08 18:25:01 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_indent(int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
}

void	print_cmd_array(char **cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1])
			printf(" ");
		i++;
	}
}

void	print_redir_type(token_type type, char *filename)
{
	if (type == T_REDIR_OUT)
		printf("> %s\n", filename);
	else if (type == T_REDIR_IN)
		printf("< %s\n", filename);
	else if (type == T_REDIR_APPEND)
		printf(">> %s\n", filename);
	else if (type == T_HEREDOC)
		printf("<< %s\n", filename);
}

void	print_ast_node(t_node *node, int depth)
{
	if (!node)
		return ;
	print_indent(depth);
	if (node->type == CMD_NODE)
	{
		printf("├─ CMD: ");
		print_cmd_array(node->cmd);
		if (node->builtin != BLT_NONE)
			printf("  [builtin]");
		printf("\n");
	}
	else if (node->type == REDIR_NODE)
	{
		printf("├─ REDIR: ");
		print_redir_type(node->redir_type, node->filename);
		print_indent(depth);
		printf("│  Left:\n");
		print_ast_node(node->left, depth + 2);
	}
	else if (node->type == PIPE_NODE)
	{
		printf("├─ PIPE\n");
		print_indent(depth);
		printf("│  Left:\n");
		print_ast_node(node->left, depth + 2);
		print_indent(depth);
		printf("│  Right:\n");
		print_ast_node(node->right, depth + 2);
	}
}

void	print_ast(t_node *root)
{
	printf("\n========== AST ==========\n");
	if (!root)
	{
		printf("(empty tree)\n");
		printf("=========================\n");
		return ;
	}
	print_ast_node(root, 0);
	printf("=========================\n\n");
}
