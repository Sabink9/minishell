/* signals.c */
#include "mini.h"
#include <readline/readline.h>
#include <unistd.h>

volatile sig_atomic_t	g_sig = 0;

void	sigint_handler(int sig)
{
	(void)sig;
	g_sig = SIGINT;
	write(1, "\n", 1);
	/* on efface la ligne en cours et on passe à une nouvelle ligne,
		PAS de rl_redisplay ici (ça évite le double $> et les états bizarres) */
	rl_replace_line("", 0);
	rl_on_new_line();
}

static void	sigquit_handler(int sig)
{
	(void)sig;
}

void	setup_interactive_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = sigint_handler;
	sigaction(SIGINT, &sa, NULL);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = sigquit_handler;
	sigaction(SIGQUIT, &sa, NULL);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
