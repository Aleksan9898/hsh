/*
 * Copyright (c) 2026 Aleksan Aleksanyan
 * SPDX-License-Identifier: MIT
 */

#ifndef HSH_H
#define HSH_H

#ifndef HAVE_READLINE
#define HAVE_READLINE 1
#endif

void hsh_loop(void);
char *hsh_read_line(void);
char **hsh_split_line(char *line);
int hsh_launch(char **args);
int hsh_execute(char **args);

#endif // HSH_H
