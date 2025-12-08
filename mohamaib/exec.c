/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:29:31 by markhali          #+#    #+#             */
/*   Updated: 2025/11/24 00:20:13 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_builtin(t_node *node, t_env **env)
{
	if (node->builtin == BLT_ECHO)
		return (builtin_echo(node->cmd));
	else if (node->builtin == BLT_PWD)
		return (builtin_pwd());
	else if (node->builtin == BLT_CD)
		return (builtin_cd(node->cmd, env));
	else if (node->builtin == BLT_ENV)
		return (builtin_env(*env));
	else if (node->builtin == BLT_EXPORT)
		return (builtin_export(env, node->cmd));
	else if (node->builtin == BLT_UNSET)
		return (builtin_unset(env, node->cmd));
	else if (node->builtin == BLT_EXIT)
		return (builtin_exit(node->cmd));
	return (0);
}

int	execute_command(t_node *node, t_env **env, t_gc *gc)
{
	if (!node || !node->cmd || !node->cmd[0])
		return (0);
	// For now, only execute built-ins
	if (node->builtin != BLT_NONE)
		return (execute_builtin(node, env));
	// External commands will be implemented later
	if(node->builtin == BLT_NONE)
		return (exec_cmd(node, env, gc));
	printf("minishell: command not found: %s\n", node->cmd[0]);
	return (127);
}

int	execute_node(t_node *node, t_env **env, t_gc *gc)
{
	if (!node)
		return (0);
	if (node->type == PIPE_NODE)
	{
		// Pipes will be implemented later
		// printf("Note: Pipeline detected but not yet implemented\n");
		// For now, just execute left side
		// if (node->left)
		// 	execute_node(node->left, env, gc);
		// if (node->right)
		// 	execute_node(node->right, env, gc);
    	return (execute_pipe(node, env, gc));
	}
	else if (node->type == REDIR_NODE)
	{
		int status;
    	pid_t pid = fork();
    	if (pid < 0)
    	{
        	perror("minishell: fork");
        	return (1);
    	}
    	if (pid == 0)
    	{
        	/* Child: apply all redirections (left-first) then execute the inner command */
        	apply_redirections(node, env, gc);
        	/* Now run the command that is wrapped by the redirection node */
        	/* execute_node(node->left, ...) will either call CMD_NODE handling or further forks */
        	int code = execute_node(node->left, env, gc);
        	/* If execute_node returned (did not execve), exit with that code */
        	exit(code);
    	}
    	/* Parent: wait for child and forward child's exit code */
    	waitpid(pid, &status, 0);
    	return (0);
	}
	else if (node->type == CMD_NODE)
		return (execute_command(node, env, gc));
	return (0);
}

void apply_redirections(t_node *node, t_env **env, t_gc *gc)
{
    int fd;

    if (!node)
        return;

    /* apply right subtree first */
    apply_redirections(node->right, env, gc);

    if (node->type == REDIR_NODE)
    {
        if (node->redir_type == T_HEREDOC)
        {
            /* Prefer prepared pipe read end, otherwise use file_fd prepared by fallback */
            fd = -1;
            if (node->heredoc_fd != -1)
                fd = node->heredoc_fd;
            else if (node->file_fd > 0)
                fd = node->file_fd;
            else
            {
                /* If not prepared, try to create fallback file (this should rarely run if
                   prepare_heredocs(root, env, gc) ran earlier). handle_redir_heredoc sets node->file_fd. */
                if (handle_redir_heredoc(node, env, gc) != 0)
                {
                    /* handle_redir_heredoc already printed error */
                    exit(1);
                }
                fd = node->file_fd;
            }

            if (fd < 0)
            {
                /* Nothing to dup — treat as error */
                write(2, "minishell: heredoc: no input\n", 29);
                exit(1);
            }

            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("minishell: dup2 (heredoc)");
                /* If fd was an open file we should close it */
                if (node->file_fd > 0 && node->file_fd == fd)
                    close(node->file_fd);
                exit(1);
            }

            /* Close the fd after dup2 to avoid fd leaks.
               If it was a prepared pipe read end, close it too. */
            if (node->heredoc_fd != -1)
            {
                close(node->heredoc_fd);
                node->heredoc_fd = -1;
            }
            if (node->file_fd > 0)
            {
                close(node->file_fd);
                node->file_fd = -1;
            }
        }
        else if (node->redir_type == T_REDIR_IN)
        {
            fd = open(node->filename, O_RDONLY);
            if (fd < 0)
            {
                perror(node->filename);
                exit(1);
            }
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("minishell: dup2 (input)");
                close(fd);
                exit(1);
            }
            close(fd);
        }
        else if (node->redir_type == T_REDIR_OUT)
        {
            fd = open(node->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror(node->filename);
                exit(1);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("minishell: dup2 (output)");
                close(fd);
                exit(1);
            }
            close(fd);
        }
        else if (node->redir_type == T_REDIR_APPEND)
        {
            fd = open(node->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror(node->filename);
                exit(1);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("minishell: dup2 (append)");
                close(fd);
                exit(1);
            }
            close(fd);
        }
    }

    /* Apply left subtree as well (usually empty in your AST but safe to include). */
    apply_redirections(node->left, env, gc);
}