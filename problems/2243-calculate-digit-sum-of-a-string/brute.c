/* Slow and obviously right: follow the rules literally, building each new
 * string in a fresh buffer and writing every total with snprintf. */
char *digitSum(char *s, int k)
{
    char *cur = malloc(strlen(s) + 1), *next;
    size_t size, len;
    int n, i, j, sum;

    strcpy(cur, s);
    while ((n = (int)strlen(cur)) > k) {
        size = (size_t)n * 4 + 1;
        next = malloc(size);
        next[0] = '\0';
        for (i = 0; i < n; i += k) {
            sum = 0;
            for (j = i; j < i + k && j < n; j++)
                sum += cur[j] - '0';
            len = strlen(next);
            snprintf(next + len, size - len, "%d", sum);
        }
        free(cur);
        cur = next;
    }
    return cur;
}
