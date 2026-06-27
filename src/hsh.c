/*
 * Copyright (c) 2026 Aleksan Aleksanyan
 * SPDX-License-Identifier: MIT
 */

#include "hsh.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifdef HAVE_READLINE
#include <readline/history.h>
#include <readline/readline.h>
#endif

#define HSH_TOK_BUFF_SIZE 16
#define HSH_TOKEN_DELIMITERS " \t\r\n\a"

#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_CYAN "\033[1;36m"

void hsh_loop()
{
    char *line;
    char **args;
    int status = 1;
    char cwd[PATH_MAX];
    char prompt[PATH_MAX + 64];

#ifdef HAVE_READLINE
    rl_bind_key('\t', rl_complete);
#endif

    while (status) {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
#ifdef HAVE_READLINE
            char *p = prompt;
            p = stpcpy(p, COLOR_GREEN "[hsh]" COLOR_RESET " -> " COLOR_BLUE);
            p = stpcpy(p, cwd);
            stpcpy(p, COLOR_RESET COLOR_CYAN " :> " COLOR_RESET);
#else
            printf("%s:> ", cwd);
            fflush(stdout);
#endif
        } else {
#ifdef HAVE_READLINE
            stpcpy(prompt, COLOR_GREEN "[hsh]" COLOR_CYAN " :> " COLOR_RESET);
#else
            perror("hsh: getcwd error");
            printf("hsh:> ");
            fflush(stdout);
#endif
        }

#ifdef HAVE_READLINE
        line = readline(prompt);
        if (line && strlen(line) > 0) {
            add_history(line);
        }
#else
        line = hsh_read_line();
#endif
        if (!line) {
            continue;
        }

        args = hsh_split_line(line);
        status = hsh_execute(args);

        free(args);
        free(line);
    }
}

char *hsh_read_line()
{
    char *line = NULL;
    size_t bufsize = 0;

    if (getline(&line, &bufsize, stdin) == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        } else {
            exit(EXIT_FAILURE);
        }
    }

    return line;
}

char **hsh_split_line(char *line)
{
    int bufsize = HSH_TOK_BUFF_SIZE;
    int count = 0;

    char **args = malloc(bufsize * sizeof(char *));
    if (!args) {
        return NULL;
    }

    char *token = strtok(line, HSH_TOKEN_DELIMITERS);
    while (token != NULL) {
        args[count++] = token;

        if (count >= bufsize - 1) {
            bufsize *= 2;
            char **temp = realloc(args, bufsize * sizeof(char *));
            if (!temp) {
                free(args);
                return NULL;
            }
            args = temp;
        }

        token = strtok(NULL, HSH_TOKEN_DELIMITERS);
    }

    args[count] = NULL;
    return args;
}

int hsh_launch(char **args)
{
    pid_t pid = fork();
    int status;

    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("hsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("hsh");
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int hsh_execute(char **args)
{
    if (args[0] == NULL) {
        return 1;
    }

    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fputs("hsh: expected argument to \"cd\"\n", stderr);
        } else if (chdir(args[1]) != 0) {
            perror("hsh");
        }
        return 1;
    }

    if (strcmp(args[0], "history") == 0) {
#ifdef HAVE_READLINE
        HIST_ENTRY **list = history_list();
        if (list) {
            for (int i = 0; list[i] != NULL; i++) {
                printf("%d  %s\n", i + history_base, list[i]->line);
            }
        }
#else
        fputs("hsh: history is disabled in standard input mode\n", stderr);
#endif
        return 1;
    }

    if (strcmp(args[0], "exit") == 0) {
        return 0;
    }

    return hsh_launch(args);
}
