#include "mini.h"
#include <readline/readline.h>
#include <unistd.h>

volatile sig_atomic_t	g_sig = 0;

void	sigint_handler(int sig)
{
	(void)sig;
	g_sig = SIGINT;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	/* >>> clé : dire à readline d’arrêter et de retourner tout de suite */
	rl_redisplay();
	rl_done = 1;
}

static void	sigquit_handler(int sig)
{
	(void)sig;
}

void	setup_interactive_signals(void)
{
	struct sigaction	sa;

	/* SIGINT: ne pas utiliser SA_RESTART pour le prompt */
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0; /* <<< au lieu de SA_RESTART */
	sa.sa_handler = sigint_handler;
	sigaction(SIGINT, &sa, NULL);
	/* SIGQUIT: tu peux garder SA_RESTART */
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
