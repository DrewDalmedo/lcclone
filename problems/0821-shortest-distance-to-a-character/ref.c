/* Reference solution: two passes. Left to right, answer[i] becomes the
 * distance to the nearest c at or before i (n, more than any distance,
 * while no c has been seen yet); right to left, it is lowered to the
 * distance to the nearest c at or after i when that is smaller. */
int *shortestToChar(char *s, char c, int *returnSize)
{
    int n = (int)strlen(s), last = -1, i;
    int *answer = malloc((size_t)n * sizeof *answer);

    for (i = 0; i < n; i++) {
        if (s[i] == c)
            last = i;
        answer[i] = last >= 0 ? i - last : n;
    }
    last = -1;
    for (i = n - 1; i >= 0; i--) {
        if (s[i] == c)
            last = i;
        if (last >= 0 && last - i < answer[i])
            answer[i] = last - i;
    }
    *returnSize = n;
    return answer;
}
