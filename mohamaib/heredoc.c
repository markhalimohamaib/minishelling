
#include "minishell.h"

static int	is_delimiter(char *line, char *delimiter)
{
	int	i;

	i = 0;
	while (delimiter[i] && line[i])
	{
		if (delimiter[i] != line[i])
			return (0);
		i++;
	}
	if (delimiter[i] == '\0' && line[i] == '\0')
		return (1);
	return (0);
}

static volatile sig_atomic_t heredoc_interrupted = 0;

static void heredoc_sigint_handler(int sig)
{
    (void)sig;
    heredoc_interrupted = 1;
}

/* Returns:
   0 = success (delimiter seen)
   1 = interrupted by SIGINT (Ctrl-C) - caller should abort execution
   2 = EOF encountered before delimiter (warning already printed) */
int write_heredoc_content(int fd, char *delimiter, int expand, t_env **env, t_gc *gc)
{
    char *line;
    char *out;
    t_segment seg;
    struct sigaction sa_old, sa_new;

    /* install simple SIGINT handler to detect Ctrl-C */
    heredoc_interrupted = 0;
    sa_new.sa_handler = heredoc_sigint_handler;
    sigemptyset(&sa_new.sa_mask);
    sa_new.sa_flags = 0;
    sigaction(SIGINT, &sa_new, &sa_old);

    while (1)
    {
        if (heredoc_interrupted)
        {
            /* user hit Ctrl-C while entering heredoc */
            sigaction(SIGINT, &sa_old, NULL);
            return (1);
        }

        line = readline("> ");
        if (!line)
        {
            /* EOF: print warning like bash and return special code 2 */
            write(2, "minishell: warning: here-document delimited by end-of-file (wanted `", 69);
            write(2, delimiter, ft_strlen(delimiter));
            write(2, "')\n", 3);
            sigaction(SIGINT, &sa_old, NULL);
            return (2);
        }
        if (is_delimiter(line, delimiter))
        {
            free(line);
            sigaction(SIGINT, &sa_old, NULL);
            return (0); /* normal */
        }

        if (expand)
        {
            seg.str = line;
            seg.seg_state = NORMAL;
            seg.expands = 1;
            out = check_for_dollar(seg, env, gc);
            if (out && out[0] != '\0')
                write(fd, out, ft_strlen(out));
            write(fd, "\n", 1);
            free(line);
            /* out is GC-managed */
        }
        else
        {
            write(fd, line, ft_strlen(line));
            write(fd, "\n", 1);
            free(line);
        }
    }
}

void collect_heredocs(t_node *node, t_list **list)
{
    if (!node) return;

    /* Pre-order traversal: visit node first so heredocs are collected
       in the parser/token appearance order (node, left, right). */
    if (node->type == REDIR_NODE && node->redir_type == T_HEREDOC)
        ft_lstadd_back(list, ft_lstnew(node));

    collect_heredocs(node->left, list);
    collect_heredocs(node->right, list);
}


/* Process heredocs IN ORDER OF APPEARANCE */
void prepare_heredocs(t_node *root, t_env **env, t_gc *gc)
{
    t_list *hdocs = NULL;
    t_list *tmp;
    int p[2];

    collect_heredocs(root, &hdocs);
    tmp = hdocs;
    while (tmp)
    {
        t_node *node = tmp->content;

        if (pipe(p) == -1)
        {
            perror("minishell: pipe");
            /* cleanup any previously prepared fds */
            // close previous nodes' heredoc_fd if necessary
            tmp = tmp->next;
            continue;
        }

        /* Fork a writer child so readline() and signal handling occur in the child
           and do not interfere with the main shell process. The child writes into
           p[1], the parent keeps p[0] as the prepared read end. */
        pid_t writer = fork();
        if (writer < 0)
        {
            perror("minishell: fork");
            close(p[0]);
            close(p[1]);
            tmp = tmp->next;
            continue;
        }

        if (writer == 0)
        {
            /* writer child */
            close(p[0]);
            int wres = write_heredoc_content(p[1], node->filename,
                                            !node->heredoc_no_expand, env, gc);
            close(p[1]);
            /* use same numeric codes as write_heredoc_content */
            if (wres == 1)
                _exit(1);
            else if (wres == 2)
                _exit(2);
            _exit(0);
        }

        /* parent */
        close(p[1]);
        int status;
        waitpid(writer, &status, 0);
        int wret = 0;
        if (WIFEXITED(status))
            wret = WEXITSTATUS(status);

        if (wret == 1)
        {
            /* interrupted by Ctrl-C while entering heredoc */
            close(p[0]);
            node->heredoc_fd = -2;
            return;
        }

        /* attach read end to node for later use (even if wret == 2: EOF warning already printed) */
        node->heredoc_fd = p[0];

        tmp = tmp->next;
    }
}

void cleanup_heredocs(t_node *node)
{
    if (!node)
        return;
    cleanup_heredocs(node->left);
    cleanup_heredocs(node->right);

    if (node->type == REDIR_NODE)
    {
        if (node->redir_type == T_HEREDOC && node->heredoc_fd >= 0)
        {
            close(node->heredoc_fd);
            node->heredoc_fd = -1;
        }
    }
}

int heredoc_was_interrupted(t_node *node)
{
    if (!node) return 0;
    if (node->type == REDIR_NODE && node->redir_type == T_HEREDOC)
    {
        if (node->heredoc_fd == -2)
            return 1;
    }
    if (heredoc_was_interrupted(node->left))
        return 1;
    if (heredoc_was_interrupted(node->right))
        return 1;
    return 0;
}
