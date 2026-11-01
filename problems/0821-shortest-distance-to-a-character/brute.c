/* Slow and obviously right, straight from the definition: for every
 * index i, look at every index j holding c and keep the smallest
 * |i - j|. */
int *shortestToChar(char *s, char c, int *returnSize)
{
    int n = (int)strlen(s), i, j, d;
    int *answer = malloc((size_t)n * sizeof *answer);

    for (i = 0; i < n; i++) {
        answer[i] = -1;
        for (j = 0; j < n; j++) {
            if (s[j] != c)
                continue;
            d = i > j ? i - j : j - i;
            if (answer[i] < 0 || d < answer[i])
                answer[i] = d;
        }
    }
    *returnSize = n;
    return answer;
}
