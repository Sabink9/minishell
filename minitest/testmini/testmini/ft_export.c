#include "mini.h"


/* Compte le nombre d'éléments dans envp */
static int envp_size(char **envp)
{
    int i = 0;
    while (envp && envp[i])
        i++;
    return i;
}

/* Cherche la position d'une variable dans envp, retourne -1 si pas trouvé */
static int find_env_index(char **envp, const char *key)
{
    int i = 0;
    int len = strlen(key);

    while (envp[i])
    {
        if (strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
            return i;
        i++;
    }
    return -1;
}

/* Affiche les variables exportées */
static void print_export(char **envp)
{
    int i = 0;
    while (envp && envp[i])
    {
        printf("declare -x %s\n", envp[i]);
        i++;
    }
}

/* Ajoute ou modifie une variable dans envp */
static char **set_env_var(char **envp, const char *key, const char *value)
{
    int idx = find_env_index(envp, key);
    int size = envp_size(envp);
    char *new_var;
    char **new_envp;

    new_var = malloc(strlen(key) + 2 + (value ? strlen(value) : 0));
    if (!new_var)
        return envp;
    if (value)
        sprintf(new_var, "%s=%s", key, value);
    else
        sprintf(new_var, "%s=", key);

    if (idx >= 0)
    {
        free(envp[idx]);
        envp[idx] = new_var;
        return envp;
    }

    /* Ajout d'une nouvelle variable */
    new_envp = malloc(sizeof(char *) * (size + 2)); // +1 pour la nouvelle, +1 pour NULL
    if (!new_envp)
    {
        free(new_var);
        return envp;
    }

    for (int i = 0; i < size; i++)
        new_envp[i] = envp[i];
    new_envp[size] = new_var;
    new_envp[size + 1] = NULL;

    free(envp); // on libère l'ancien tableau
    return new_envp;
}

/* Sépare key et value depuis une chaîne "KEY=VALUE" */
static void split_key_value(const char *str, char **key, char **value)
{
    int i = 0;
    while (str[i] && str[i] != '=')
        i++;
    *key = strndup(str, i);
    if (str[i] == '=')
        *value = strdup(str + i + 1);
    else
        *value = NULL;
}

/* ----------------- EXPORT ----------------- */
char **ft_export(char **envp, char **args)
{
    int i = 1;
    char *key;
    char *value;

    if (!args[1])
    {
        print_export(envp);
        return envp;
    }

    while (args[i])
    {
        split_key_value(args[i], &key, &value);
        envp = set_env_var(envp, key, value);
        free(key);
        free(value);
        i++;
    }
    return envp;
}
