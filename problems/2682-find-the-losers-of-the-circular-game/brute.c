/* Slow and obviously right: move the frog one pad at a time with 1-based
 * labels, t * k single steps on jump t, count how many times it lands on
 * each pad, and stop when some pad's count reaches 2. */
int *circularGameLosers(int n, int k, int *returnSize)
{
    int landed[51] = {0};
    int *out = malloc((size_t)n * sizeof *out);
    int pad = 1, t, step, m = 0, i;

    landed[1] = 1;
    for (t = 1; landed[pad] < 2; t++) {
        for (step = 0; step < t * k; step++)
            pad = pad == n ? 1 : pad + 1;
        landed[pad]++;
    }
    for (i = 1; i <= n; i++)
        if (landed[i] == 0)
            out[m++] = i;
    *returnSize = m;
    return out;
}
