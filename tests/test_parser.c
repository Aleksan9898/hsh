#include "hsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failed_tests_count = 0;

int verify_tokens(char **result, const char **expected, int expected_count)
{
    if (result == NULL && expected == NULL)
        return 1;
    if (result == NULL || expected == NULL)
        return 0;

    for (int i = 0; i < expected_count; i++) {
        if (result[i] == NULL)
            return 0;
        if (strcmp(result[i], expected[i]) != 0)
            return 0;
    }

    if (result[expected_count] != NULL)
        return 0;

    return 1;
}

void run_test(const char *test_name,
              const char *input_line,
              const char **expected,
              int expected_count)
{
    printf("Running %s... ", test_name);

    char *line_copy = strdup(input_line);
    if (!line_copy) {
        printf("\033[0;31mFAILED (OOM)\033[0m\n");
        failed_tests_count++;
        return;
    }

    char **result = hsh_split_line(line_copy);

    if (verify_tokens(result, expected, expected_count)) {
        printf("\033[0;32mPASSED\033[0m\n");
    } else {
        printf("\033[0;31mFAILED\033[0m\n");
        printf("  Input line: \"%s\"\n", input_line);
        failed_tests_count++;
    }

    free(result);
    free(line_copy);
}

int main(void)
{
    printf("=== STARTING UNIT TESTS FOR hsh_splie_line ===\n\n");

    {
        const char *input = "ls -l /home/user";
        const char *expected[] = {"ls", "-l", "/home/user"};
        run_test("Test 1: Standard command", input, expected, 3);
    }

    {
        const char *input = "   mkdir    -p   \t  /data/db  \n ";
        const char *expected[] = {"mkdir", "-p", "/data/db"};
        run_test("Test 2: Multiple spaces and tabs", input, expected, 3);
    }

    {
        const char *input = "   \n  \t  ";
        const char *expected[] = {NULL};
        run_test("Test 3: Empty spaces line", input, expected, 0);
    }

    {
        const char *input = "clear";
        const char *expected[] = {"clear"};
        run_test("Test 4: Single word", input, expected, 1);
    }

    {
        const char *input = "cat\rfile.txt";
        const char *expected[] = {"cat", "file.txt"};
        run_test("Test 5: Carriage return delimiter", input, expected, 2);
    }

    printf("\n=== TESTING COMPLETED ===\n");

    if (failed_tests_count > 0) {
        printf("Total errors found: %d\n", failed_tests_count);
        return 1;
    }

    printf("All tests passed successfully!\n");
    return 0;
}
