/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 16:46:03 by markhali          #+#    #+#             */
/*   Updated: 2025/12/16 19:50:46 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Global variable to store signal number (as required by subject) */
volatile sig_atomic_t g_signal = 0;

/*
 * Signal handler for interactive mode (waiting for user input)
 * Handles SIGINT (ctrl-C)
 */
void	handle_sigint_interactive(int sig)
{
	g_signal = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*
 * Signal handler for command execution mode
 * Handles SIGINT (ctrl-C) during command execution
 */
void	handle_sigint_exec(int sig)
{
	g_signal = sig;
	write(STDOUT_FILENO, "\n", 1);
}

/*
 * Signal handler for heredoc mode
 * Handles SIGINT (ctrl-C) during heredoc input
 */
void	handle_sigint_heredoc(int sig)
{
	g_signal = sig;
	rl_done = 1;
}

/*
 * Setup signals for interactive mode (main prompt)
 * - SIGINT (ctrl-C): Display new prompt on new line
 * - SIGQUIT (ctrl-\): Ignore
 */
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

/*
 * Setup signals for command execution mode
 * - SIGINT (ctrl-C): Print newline, let child handle termination
 * - SIGQUIT (ctrl-\): Let default behavior handle (prints "Quit")
 */
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

/*
 * Setup signals for heredoc input mode
 * - SIGINT (ctrl-C): Exit heredoc with status 130
 */
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

/*
 * Setup default signal handlers for child processes
 */
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

/*
 * Get the exit status from the global signal variable
 * Returns 130 for SIGINT (ctrl-C), 131 for SIGQUIT (ctrl-\)
 */
int	get_signal_exit_status(void)
{
	if (g_signal == SIGINT)
		return (130);
	if (g_signal == SIGQUIT)
		return (131);
	return (0);
}

/*
 * Reset the global signal variable
 */
void	reset_signal(void)
{
	g_signal = 0;
}
