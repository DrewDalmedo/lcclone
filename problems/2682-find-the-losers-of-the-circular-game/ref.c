/* Reference solution: follow the frog with 0-based positions, adding
 * t * k modulo n on jump t and marking each pad, until a jump lands on a
 * marked pad; then list the unmarked pads in order, as 1-based labels. */
int *circularGameLosers(int n, int k, int *returnSize)
{
    bool seen[50] = {false};
    int *out = malloc((size_t)n * sizeof *out);
    int pos = 0, t, m = 0, i;

    seen[0] = true;
    for (t = 1;; t++) {
        pos = (pos + t * k) % n;
        if (seen[pos])
            break;
        seen[pos] = true;
    }
    for (i = 0; i < n; i++)
        if (!seen[i])
            out[m++] = i + 1;
    *returnSize = m;
    return out;
}
