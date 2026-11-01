/* Reference solution: read command from the left. A 'G' is copied; at a
 * '(' the next character tells the token apart: ')' means "()", written
 * as 'o', and 'a' means "(al)", written as "al". No token expands to more
 * characters than it has, so command's length plus 1 is room enough. */
char *interpret(char *command)
{
    size_t n = strlen(command), i = 0, len = 0;
    char *out = malloc(n + 1);

    while (i < n) {
        if (command[i] == 'G') {
            out[len++] = 'G';
            i += 1;
        } else if (command[i + 1] == ')') {
            out[len++] = 'o';
            i += 2;
        } else {
            out[len++] = 'a';
            out[len++] = 'l';
            i += 4;
        }
    }
    out[len] = '\0';
    return out;
}
