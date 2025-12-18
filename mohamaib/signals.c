/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 16:46:03 by markhali          #+#    #+#             */
/*   Updated: 2025/12/18 19:53:59 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal = 0;

void	handle_sigint_interactive(int sig)
{
	g_signal = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigint_exec(int sig)
{
	g_signal = sig;
	write(STDOUT_FILENO, "\n", 1);
}

void	handle_sigint_heredoc(int sig)
{
	g_signal = sig;
	rl_done = 1;
}

void	setup_signals_interactive(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = handle_sigint_interactive;
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);

	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_signals_exec(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = handle_sigint_exec;
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_signals_heredoc(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = handle_sigint_heredoc;
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_signals_child(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = SIG_DFL;
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = SIG_DFL;
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

int	get_signal_exit_status(void)
{
	if (g_signal == SIGINT)
		return (130);
	if (g_signal == SIGQUIT)
		return (131);
	return (0);
}

void	reset_signal(void)
{
	g_signal = 0;
}
