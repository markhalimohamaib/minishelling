/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 18:51:06 by mohamaib          #+#    #+#             */
/*   Updated: 2025/10/23 22:41:31 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to print indentation
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

// Helper function to print redirections
static void	print_redirs(t_token *redir, int depth)
{
	if (!redir)
		return;
	
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

// Recursive function to print AST
static void	print_ast_recursive(t_node *node, int depth)
{
	int	i;

	if (!node)
		return;

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

// Main function to print the entire AST
void	print_ast(t_node *root)
{
	printf("\n");
	printf("========== AST ==========\n");
	if (!root)
	{
		printf("(empty tree)\n");
		printf("=========================\n");
		return;
	}
	
	print_ast_recursive(root, 0);
	printf("=========================\n");
	printf("\n");
}

// Alternative: More compact single-line view
void	print_ast_compact(t_node *node)
{
	int	i;

	if (!node)
		return;
	
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
	int		i;

	if (!node)
	{
		printf("(null node)\n");
		return;
	}

	printf("=== NODE ===\n");
	printf("Type: ");
	if (node->type == CMD_NODE)
		printf("CMD\n");
	else if (node->type == PIPE_NODE)
		printf("PIPE\n");
	else if (node->type == REDIR_NODE)
		printf("REDIR\n");
	else
		printf("UNKNOWN\n");

	// Print command array (CMD_NODE only)
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

	// Print redirection info (REDIR_NODE only)
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

	// Print tree pointers
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

int		count_until_pipe(t_token **head)
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

char	**create_cmd_from_tokens(t_token **head)
{
	int		i;
	char	**cmd;
	t_token	*tmp;

	i = 0;
	tmp = (*head);
	cmd = malloc(sizeof(char *) * (count_until_pipe(head) + 1));
	while(tmp)
	{
		if(tmp->type == T_PIPE)
		{
			cmd[i] = NULL;
			return (cmd);
		}
		if (tmp->type == T_WORD)
		{
			cmd[i] = ft_strdup(tmp->value);
			i++;
		}
		tmp = tmp->next;
	}
	cmd[i] = NULL;
	return (cmd);
}

// t_token	*copy_redir(t_token *tmp)
// {
// 	t_token	*redir;
	
// 	redir = malloc(sizeof(t_token));
// 	redir->type = tmp->type;
// 	redir->next = NULL;
// 	if(tmp->filename)
// 		redir->filename = ft_strdup(tmp->filename);
// 	else
// 		redir->filename = NULL;
// 	if(tmp->value)
// 		redir->value = ft_strdup(tmp->value);
// 	else
// 		redir->value = NULL;
// 	if(tmp->heredoc_del)
// 		redir->heredoc_del = ft_strdup(tmp->heredoc_del);
// 	else
// 		redir->heredoc_del = NULL;
// 	return (redir);
// }

// t_token	*get_redir(t_token **head)
// {
// 	t_token	*tmp;
// 	t_token	*redir;
	
// 	tmp = (*head);
// 	while(tmp && tmp->type != T_PIPE)
// 	{
// 		if (tmp->type == T_REDIR_IN || tmp->type == T_REDIR_OUT ||
// 			tmp->type == T_REDIR_APPEND || tmp->type == T_HEREDOC)
// 		{
// 			// tmp = malloc(sizeof(t_token));
// 			redir = copy_redir(tmp);
// 			return(redir);		
// 		}
// 		tmp = tmp->next;	
// 	}
// 	return (NULL);
// }

t_node	*create_nodes(t_token **head, char **cmd)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	node->type = CMD_NODE;
	node->cmd = cmd;
	// node->redirs = get_redir(head);
	node->right = NULL;
	node->left = NULL;
	return (node);
}

t_node	*create_redir_node(token_type type, char *filename, t_node *left)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	node->type = REDIR_NODE;
	node->cmd = NULL;
	node->filename = ft_strdup(filename);
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

t_node	*parse_smpl_cmd(t_token **head)
{
	char	**cmd;
	t_node	*node;
	t_node	*redir_node;
	t_token	*tmp;

	tmp = (*head);
	cmd = create_cmd_from_tokens(head);
	node = create_nodes(head, cmd);
	while (tmp && tmp->type != T_PIPE)
	{
		if (tmp->type == T_REDIR_IN || tmp->type == T_REDIR_OUT ||
			tmp->type == T_REDIR_APPEND || tmp->type == T_HEREDOC)
		{
			if (tmp->filename)
				redir_node = create_redir_node(tmp->type, tmp->filename, node);
			else if (tmp->heredoc_del)
				redir_node = create_redir_node(tmp->type, tmp->heredoc_del, node);
			else
				redir_node = create_redir_node(tmp->type, "", node);
			node = redir_node;
		}
		tmp = tmp->next;
	}
	return (node);
}

// t_node	*create_pipe_node(t_node *node, t_token **head)
// {
// 	t_node	*pipe_node;
// 	t_token	*tmp;

// 	tmp = (*head);
// 	if (tmp && tmp->type == T_PIPE)
// 	{
// 		tmp = tmp->next;
// 		pipe_node = malloc(sizeof(t_node));
// 		pipe_node->left = node;
// 		pipe_node->type = PIPE_NODE;
// 		pipe_node->right = create_pipe_node(node, &tmp);
// 		pipe_node->redirs = NULL;
// 		pipe_node->cmd = NULL;
// 		print_node(pipe_node);
// 		return (pipe_node);
// 	}
// 	// print_node(node);
// 	return (node);
// }

// t_node	*build_ast(t_token **head)
// {
// 	t_node	*pipe_node;
// 	t_token	*tmp;

	
// }

t_node	*parse_cmd(t_token **head)
{
	// char	**command;
	t_node	*pipe_node;
	t_node	*node;
	t_token	*tmp;
	// t_token	*redir;

	if ((*head)->type == T_PIPE)
		return (NULL);
	// if (!(*head))
	// 	return ;
	tmp = (*head);
	// redir = malloc(sizeof(t_token));
	// if (tmp->type == T_REDIR_IN || tmp->type == T_REDIR_OUT ||
	// 	tmp->type == T_REDIR_APPEND || tmp->type == T_HEREDOC)
	// 	redir = tmp;
	// command = create_cmd_from_tokens(head);
	node = parse_smpl_cmd(head);
	print_node(node);
	skip_to_pipe(head);
	if ((*head) && (*head)->type == T_PIPE)
	{
		// start creating pipe nodes here and start to fill tree
		(*head) = (*head)->next;
		pipe_node = malloc(sizeof(t_node));
		pipe_node->type = PIPE_NODE;
		pipe_node->left = node;
		// pipe_node->redirs = NULL;
		pipe_node->cmd = NULL;
		pipe_node->right = parse_cmd(head);
		print_node(pipe_node);
		return (pipe_node);
	}
	return(node);
}
