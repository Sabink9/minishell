#include "../libft/libft.h"
#include "mini.h"
#include <unistd.h> /* write */

/* --- helpers ------------------------------------------------------------ */

static int	is_valid_ident(const char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	if (!(s[0] == '_' || (s[0] >= 'A' && s[0] <= 'Z') || (s[0] >= 'a'
				&& s[0] <= 'z')))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!(s[i] == '_' || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a'
					&& s[i] <= 'z') || (s[i] >= '0' && s[i] <= '9')))
			return (0);
		i++;
	}
	return (1);
}

/* renvoie 1 si entry "NAME=..." matche exactement key "NAME" */
static int	entry_matches_key(const char *entry, const char *key)
{
	int	i;

	i = 0;
	if (!entry || !key)
		return (0);
	while (key[i] && entry[i] && entry[i] == key[i])
		i++;
	/* match si key fini ET entry a '=' juste après */
	return (key[i] == '\0' && entry[i] == '=');
}

static void	print_unset_error(const char *arg)
{
	write(2, "minishell: unset: `", 20);
	if (arg)
		write(2, arg, ft_strlen((char *)arg));
	write(2, "': not a valid identifier\n", 26);
}

/* retire TOUTES les occurrences de key dans envp -> retourne un nouveau envp */
static int	count_kept_entries(char **envp, const char *key)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (envp && envp[i])
	{
		if (!entry_matches_key(envp[i], key))
			count++;
		i++;
	}
	return (count);
}

static void	copy_kept_entries(char **out, char **envp, const char *key)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (envp && envp[i])
	{
		if (!entry_matches_key(envp[i], key))
		{
			out[j] = ft_strdup(envp[i]);
			j++;
		}
		i++;
	}
	out[j] = NULL;
}

static char	**env_remove_key(char **envp, const char *key)
{
	int		keep;
	char	**out;

	keep = count_kept_entries(envp, key);
	out = malloc(sizeof(char *) * (keep + 1));
	if (!out)
		return (NULL);
	copy_kept_entries(out, envp, key);
	free_split(envp);
	return (out);
}

/* --- builtin ------------------------------------------------------------ */
/* usage:  int st = ft_unset(argv, &envp);  (st=0 ok,
		st=1 si identifiant invalide) */
int	ft_unset(char **args, char ***penvp)
{
	int	i;
	int	status;

	if (!args || !args[0] || !penvp || !*penvp)
		return (0);
	status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_ident(args[i]))
		{
			print_unset_error(args[i]);
			status = 1;
		}
		else
			*penvp = env_remove_key(*penvp, args[i]);
		i++;
	}
	return (status);
}

static int	count_env_entries(char **envp)
{
	int	count;

	count = 0;
	while (envp && envp[count])
		count++;
	return (count);
}

static void	free_dup_until(char **out, int i)
{
	while (i > 0)
	{
		free(out[i - 1]);
		i--;
	}
	free(out);
}

char	**ft_env_dup(char **envp)
{
	char	**out;
	int		count;
	int		i;

	count = count_env_entries(envp);
	out = (char **)malloc(sizeof(char *) * (count + 1));
	if (!out)
		return (NULL);
	i = 0;
	while (i < count)
	{
		out[i] = ft_strdup(envp[i]);
		if (!out[i])
			return (free_dup_until(out, i), NULL);
		i++;
	}
	out[count] = NULL;
	return (out);
}
