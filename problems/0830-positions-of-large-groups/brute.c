/* Slow and obviously right, straight from the definition: try every pair
 * [a, b] with at least 3 letters, and keep it when all of s[a..b] is one
 * letter and the stretch can't be made longer on either side. Pairs are
 * tried with a increasing, so they come out in order. */
int **largeGroupPositions(char *s, int *returnSize, int **returnColumnSizes)
{
    int n = (int)strlen(s), m = 0, a, b, k, ok;
    int **out = malloc((size_t)(n + 1) * sizeof *out);

    *returnColumnSizes = malloc((size_t)(n + 1) * sizeof **returnColumnSizes);
    for (a = 0; a < n; a++)
        for (b = a + 2; b < n; b++) {
            ok = (a == 0 || s[a - 1] != s[a]) &&
                 (b == n - 1 || s[b + 1] != s[b]);
            for (k = a; ok && k <= b; k++)
                if (s[k] != s[a])
                    ok = 0;
            if (ok) {
                out[m] = malloc(2 * sizeof **out);
                out[m][0] = a;
                out[m][1] = b;
                (*returnColumnSizes)[m] = 2;
                m++;
            }
        }
    *returnSize = m;
    return out;
}
