/* Reference solution: one pass from 1 to n, picking each string by
 * divisibility, with 15 tested first. Each string gets its own malloc'ed
 * copy. */
char **fizzBuzz(int n, int *returnSize)
{
    char **out = malloc((size_t)n * sizeof *out);
    char buf[16];
    const char *call;
    int i;

    for (i = 1; i <= n; i++) {
        if (i % 15 == 0) {
            call = "FizzBuzz";
        } else if (i % 3 == 0) {
            call = "Fizz";
        } else if (i % 5 == 0) {
            call = "Buzz";
        } else {
            snprintf(buf, sizeof buf, "%d", i);
            call = buf;
        }
        out[i - 1] = malloc(strlen(call) + 1);
        strcpy(out[i - 1], call);
    }
    *returnSize = n;
    return out;
}
