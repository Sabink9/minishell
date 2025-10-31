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
	rl_redisplay();
}

static void	sigquit_handler(int s)
{
	(void)s;
	/* En interactif, rien à afficher */
}

void	setup_interactive_signals(void)
{
	struct sigaction	sa;

	/* SIGINT */
	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	/* SIGQUIT */
	sa.sa_handler = sigquit_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa, NULL);
}

/* Dans les enfants : comportement par défaut (comme bash) */
void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
