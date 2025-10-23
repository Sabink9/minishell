/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kberraho <kberraho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:08:07 by kberraho          #+#    #+#             */
/*   Updated: 2025/10/23 13:17:24 by kberraho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	handle_sigint(int sig)
{
	(void)sig;
	printf("\n");           // saute une ligne
	rl_replace_line("", 0); // efface la ligne en cours
	rl_on_new_line();       // prépare une nouvelle ligne rl_redisplay();
	rl_redisplay();         // réaffiche le prompt
}

void	handle_sigquit(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
}

void	init_signals(void)
{
	signal(SIGINT, handle_sigint);   // ctrl-C
	signal(SIGQUIT, handle_sigquit); // ctrl-D
	signal(SIGQUIT, SIG_IGN);        // ctrl-*\*
}
