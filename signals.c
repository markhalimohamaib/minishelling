/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 16:46:03 by markhali          #+#    #+#             */
/*   Updated: 2025/12/23 20:01:50 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

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
