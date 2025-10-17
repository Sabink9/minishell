#include "mini.h"

char *get_path_from_env(char **envp)
{
    int i = 0;
    char *eq;
    while (envp[i])
    {
        eq = strchr(envp[i], '=');
        if (eq && strncmp(envp[i], "PATH", 4) == 0)
            return eq + 1;
        i++;
    }
    return NULL;
}

char *find_executable(char *cmd, char **envp)
{
    char *path_env = get_path_from_env(envp);
    char *path_dup;
    char *token;
    char *full;
    int len;

    if (!path_env)
        return NULL;

    if (strchr(cmd, '/'))
        return strdup(cmd);

    path_dup = strdup(path_env);
    token = strtok(path_dup, ":");
    while (token)
    {
        len = strlen(token) + 1 + strlen(cmd) + 1;
        full = malloc(len);
        snprintf(full, len, "%s/%s", token, cmd);
        if (access(full, X_OK) == 0)
        {
            free(path_dup);
            return full;
        }
        free(full);
        token = strtok(NULL, ":");
    }
    free(path_dup);
    return NULL;
}

void exec_command(char **args, char **envp)
{
    pid_t pid;
    char *exec_path = find_executable(args[0], envp);

    if (!exec_path)
    {
        printf("minishell: command not found: %s\n", args[0]);
        return;
    }

    pid = fork();
    if (pid == 0)
        execve(exec_path, args, envp);
    else
        waitpid(pid, NULL, 0);

    free(exec_path);
}