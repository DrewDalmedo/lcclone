/* Reference solution: go through all 12 * 60 times on the watch and keep
 * those whose hour and minute together have turnedOn bits set. */
static int bits_set(int v)
{
    int count = 0;

    while (v) {
        v &= v - 1;
        count++;
    }
    return count;
}

char **readBinaryWatch(int turnedOn, int *returnSize)
{
    char **out = malloc(12 * 60 * sizeof *out);
    int h, m, n = 0;

    for (h = 0; h < 12; h++)
        for (m = 0; m < 60; m++)
            if (bits_set(h) + bits_set(m) == turnedOn) {
                out[n] = malloc(6);
                snprintf(out[n], 6, "%d:%02d", h, m);
                n++;
            }
    *returnSize = n;
    return out;
}
