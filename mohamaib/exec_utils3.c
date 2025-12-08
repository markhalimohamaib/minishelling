/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 20:02:33 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/05 23:44:07 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redir_in(t_node *node, t_env **env, t_gc *gc)
{
    int	exit_code;
    
    node->file_fd = open(node->filename, O_RDONLY);
    if (node->file_fd < 0)
    {
        perror(node->filename);
        return (127);
    }
    node->pid1 = fork();
    if (node->pid1 == 0)
    {
        dup2(node->file_fd, STDIN_FILENO);
        close(node->file_fd);
        execute_node(node->left, env, gc);
        exit(0);
    }
    waitpid(node->pid1, &node->status, 0);
    exit_code = (node->status >> 8);
    close(node->file_fd);
    return (exit_code);
}

int	handle_redir_out(t_node *node, t_env **env, t_gc *gc)
{
    int	exit_code;
    
    node->file_fd = open(node->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (node->file_fd < 0)
    {
        exit(1);
    }
    node->pid1 = fork();
    if (node->pid1 == 0)
    {
        dup2(node->file_fd, STDOUT_FILENO);
        close(node->file_fd);
        execute_node(node->left, env, gc);
        exit(0);
    }
    waitpid(node->pid1, &node->status, 0);
    exit_code = (node->status >> 8);
    close(node->file_fd);
    return (exit_code);
}

int	handle_redir_append(t_node *node, t_env **env, t_gc *gc)
{
    int	exit_code;
    
    node->file_fd = open(node->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (node->file_fd < 0)
    {
        exit(1);
    }
    node->pid1 = fork();
    if (node->pid1 == 0)
    {
        dup2(node->file_fd, STDOUT_FILENO);
        close(node->file_fd);
        execute_node(node->left, env, gc);
        exit(0);
    }
    waitpid(node->pid1, &node->status, 0);
    exit_code = (node->status >> 8);
    close(node->file_fd);
    return (exit_code);
}

int	handle_redir_heredoc(t_node *node, t_env **env, t_gc *gc)
{
    char template[] = "/tmp/minishell_heredoc_XXXXXX";
    int fd_tmp = mkstemp(template);

    if (fd_tmp < 0)
        return (perror("mkstemp"), 1);

    write_heredoc_content(fd_tmp, node->filename,
                          !node->heredoc_no_expand, env, gc);
    close(fd_tmp);

    node->file_fd = open(template, O_RDONLY);
    unlink(template);
    return 0;
}
