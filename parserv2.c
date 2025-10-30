/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserv2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 18:51:06 by mohamaib          #+#    #+#             */
/*   Updated: 2025/10/30 23:33:36 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_indent(int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
}

static void	print_redirs(t_token *redir, int depth)
{
	if (!redir)
		return ;
	print_indent(depth);
	printf("Redirections:\n");
	while (redir)
	{
		print_indent(depth + 1);
		if (redir->type == T_REDIR_OUT)
			printf("> %s\n", redir->filename);
		else if (redir->type == T_REDIR_IN)
			printf("< %s\n", redir->filename);
		else if (redir->type == T_REDIR_APPEND)
			printf(">> %s\n", redir->filename);
		else if (redir->type == T_HEREDOC)
			printf("<< %s\n", redir->heredoc_del);
		redir = redir->next;
	}
}

static void	print_ast_recursive(t_node *node, int depth)
{
	int	i;

	if (!node)
		return ;
	print_indent(depth);
	if (node->type == CMD_NODE)
	{
		printf("├─ CMD: ");
		if (node->cmd)
		{
			i = 0;
			while (node->cmd[i])
			{
				printf("%s", node->cmd[i]);
				if (node->cmd[i + 1])
					printf(" ");
				i++;
			}
		}
		printf("\n");
	}
	else if (node->type == REDIR_NODE)
	{
		printf("├─ REDIR: ");
		if (node->redir_type == T_REDIR_OUT)
			printf("> %s\n", node->filename);
		else if (node->redir_type == T_REDIR_IN)
			printf("< %s\n", node->filename);
		else if (node->redir_type == T_REDIR_APPEND)
			printf(">> %s\n", node->filename);
		else if (node->redir_type == T_HEREDOC)
			printf("<< %s\n", node->filename);
		print_indent(depth);
		printf("│  Left:\n");
		print_ast_recursive(node->left, depth + 2);
	}
	else if (node->type == PIPE_NODE)
	{
		printf("├─ PIPE\n");
		print_indent(depth);
		printf("│  Left:\n");
		print_ast_recursive(node->left, depth + 2);
		print_indent(depth);
		printf("│  Right:\n");
		print_ast_recursive(node->right, depth + 2);
	}
}

void	print_ast(t_node *root)
{
	printf("\n");
	printf("========== AST ==========\n");
	if (!root)
	{
		printf("(empty tree)\n");
		printf("=========================\n");
		return ;
	}
	print_ast_recursive(root, 0);
	printf("=========================\n");
	printf("\n");
}

void	print_ast_compact(t_node *node)
{
	int	i;

	if (!node)
		return ;
	if (node->type == PIPE_NODE)
	{
		printf("(");
		print_ast_compact(node->left);
		printf(" | ");
		print_ast_compact(node->right);
		printf(")");
	}
	else if (node->type == REDIR_NODE)
	{
		if (node->redir_type == T_REDIR_OUT)
			printf("(> %s ", node->filename);
		else if (node->redir_type == T_REDIR_IN)
			printf("(< %s ", node->filename);
		else if (node->redir_type == T_REDIR_APPEND)
			printf("(>> %s ", node->filename);
		else if (node->redir_type == T_HEREDOC)
			printf("(<< %s ", node->filename);
		print_ast_compact(node->left);
		printf(")");
	}
	else if (node->type == CMD_NODE)
	{
		printf("[");
		if (node->cmd)
		{
			i = 0;
			while (node->cmd[i])
			{
				printf("%s", node->cmd[i]);
				if (node->cmd[i + 1])
					printf(" ");
				i++;
			}
		}
		printf("]");
	}
}

// -----------------------------------------------------------------------

void	print_node(t_node *node)
{
	int	i;

	if (!node)
	{
		printf("(null node)\n");
		return ;
	}
	printf("==== NODE ====\n");
	printf("Type: ");
	if (node->type == CMD_NODE)
		printf("CMD\n");
	else if (node->type == PIPE_NODE)
		printf("PIPE\n");
	else if (node->type == REDIR_NODE)
		printf("REDIR\n");
	else
		printf("UNKNOWN\n");
	if (node->cmd)
	{
		printf("cmd: ");
		i = 0;
		while (node->cmd[i])
		{
			printf("\"%s\" ", node->cmd[i]);
			i++;
		}
		printf("\n");
	}
	else
		printf("cmd: (none)\n");
	if (node->type == REDIR_NODE)
	{
		printf("redir: ");
		if (node->redir_type == T_REDIR_OUT)
			printf(">  %s\n", node->filename);
		else if (node->redir_type == T_REDIR_IN)
			printf("<  %s\n", node->filename);
		else if (node->redir_type == T_REDIR_APPEND)
			printf(">> %s\n", node->filename);
		else if (node->redir_type == T_HEREDOC)
			printf("<< %s\n", node->filename);
	}
	else if (node->filename)
		printf("filename: %s\n", node->filename);
	printf("left:  %p", (void *)node->left);
	if (node->left)
	{
		if (node->left->type == CMD_NODE)
			printf(" (CMD)");
		else if (node->left->type == PIPE_NODE)
			printf(" (PIPE)");
		else if (node->left->type == REDIR_NODE)
			printf(" (REDIR)");
	}
	printf("\n");
	printf("right: %p", (void *)node->right);
	if (node->right)
	{
		if (node->right->type == CMD_NODE)
			printf(" (CMD)");
		else if (node->right->type == PIPE_NODE)
			printf(" (PIPE)");
		else if (node->right->type == REDIR_NODE)
			printf(" (REDIR)");
	}
	printf("\n");
	printf("===============\n");
}

// ------------------------------------------------------------------------

int	count_until_pipe(t_token **head)
{
	int		count;
	t_token	*tmp;

	count = 0;
	tmp = (*head);
	while (tmp && tmp->type != T_PIPE)
	{
		tmp = tmp->next;
		count++;
	}
	return (count);
}

char	**create_cmd_from_tokens(t_token **head, t_gc *gc)
{
	int		i;
	char	**cmd;
	t_token	*tmp;

	i = 0;
	tmp = (*head);
	cmd = gc_malloc((sizeof(char *) * (count_until_pipe(head) + 1)), gc);
	while (tmp)
	{
		if (tmp->type == T_PIPE)
		{
			cmd[i] = NULL;
			return (cmd);
		}
		if (tmp->type == T_WORD)
		{
			cmd[i] = gc_ft_strdup(tmp->value, gc);
			i++;
		}
		tmp = tmp->next;
	}
	cmd[i] = NULL;
	return (cmd);
}

t_node	*create_nodes(t_token **head, char **cmd, t_gc *gc)
{
	t_node	*node;

	node = gc_malloc(sizeof(t_node), gc);
	node->type = CMD_NODE;
	node->cmd = cmd;
	node->filename = NULL;
	node->redir_type = 0;
	node->right = NULL;
	node->left = NULL;
	return (node);
}

t_node	*create_redir_node(token_type type, char *filename, t_node *left, t_gc *gc)
{
	t_node	*node;

	node = gc_malloc(sizeof(t_node), gc);
	node->type = REDIR_NODE;
	node->cmd = NULL;
	node->filename = gc_ft_strdup(filename, gc);
	node->redir_type = type;
	node->left = left;
	node->right = NULL;
	return (node);
}

void	skip_to_pipe(t_token **head)
{
	while (*head && (*head)->type != T_PIPE)
		*head = (*head)->next;
}

t_token	*find_last_redir(t_token *start)
{
	t_token	*tmp;
	t_token	*last_redir;

	tmp = start;
	last_redir = NULL;
	while (tmp && tmp->type != T_PIPE)
	{
		if (tmp->type == T_REDIR_IN || tmp->type == T_REDIR_OUT
			|| tmp->type == T_REDIR_APPEND || tmp->type == T_HEREDOC)
			last_redir = tmp;
		tmp = tmp->next;
	}
	return (last_redir);
}

t_node	*check_type(t_token *redir, t_node *node, t_gc *gc)
{
	if (redir->filename)
		return (create_redir_node(redir->type, redir->filename, node, gc));
	else if (redir->heredoc_del)
		return (create_redir_node(redir->type, redir->heredoc_del, node, gc));
	else
		return (create_redir_node(redir->type, "", node, gc));
}

t_node	*parse_smpl_cmd(t_token **head, t_gc *gc)
{
	t_node	*node;
	t_token	*start;
	t_token	*last_redir;

	start = (*head);
	node = create_nodes(head, create_cmd_from_tokens(head, gc), gc);
	last_redir = find_last_redir(start);
	while (last_redir)
	{
		node = check_type(last_redir, node, gc);
		last_redir->type = T_EOF;
		last_redir = find_last_redir(start);
	}
	while (start && start->type != T_PIPE)
		start = start->next;
	*head = start;
	return (node);
}

t_node	*parse_cmd(t_token **head, t_gc *gc)
{
	t_node	*pipe_node;
	t_node	*node;
	t_token	*tmp;

	if ((*head)->type == T_PIPE)
		return (NULL);
	tmp = (*head);
	node = parse_smpl_cmd(head, gc);
	print_node(node);
	skip_to_pipe(head);
	if ((*head) && (*head)->type == T_PIPE)
	{
		(*head) = (*head)->next;
		pipe_node = gc_malloc(sizeof(t_node), gc);
		pipe_node->type = PIPE_NODE;
		pipe_node->left = node;
		pipe_node->cmd = NULL;
		pipe_node->filename = NULL;
		pipe_node->redir_type = 0;
		pipe_node->right = parse_cmd(head, gc);
		print_node(pipe_node);
		return (pipe_node);
	}
	return (node);
}
