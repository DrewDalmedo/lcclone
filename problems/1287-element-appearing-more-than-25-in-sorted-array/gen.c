/* A sorted array with one frequent block, n = arr.length and q = n / 4
 * (rounded down). The frequent value gets from q + 1 copies (often
 * exactly q + 1, the fewest allowed) up to n; the other values get blocks
 * of 1 to q copies: all of exactly q (a quarter, just too short), all
 * single, random, or a mix. The frequent block often comes first or last.
 * Values increase from block to block, starting at 0, ending at 10^5 or
 * neither; small cases keep the gaps between them tiny. Full-size cases
 * have n = 10^4. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 16) : 10000;
    int q = n / 4, blocks = 0, at = 0, w, rest, len, pos, gap, v, i, k;
    int pick = (int)rnd(0, 4), mode = (int)rnd(0, 3), ends = (int)rnd(0, 2);
    int *size = malloc(((size_t)n + 1) * sizeof *size);
    int *vals = malloc(((size_t)n + 1) * sizeof *vals);
    int *arr = malloc((size_t)n * sizeof *arr);

    if (q == 0 || pick == 0)
        w = n;
    else if (pick <= 2)
        w = q + 1;
    else
        w = (int)rnd(q + 1, n);
    for (rest = n - w; rest > 0; rest -= len) {
        k = mode == 3 ? (int)rnd(0, 2) : mode;
        len = k == 0 ? q : k == 1 ? 1 : (int)rnd(1, q);
        if (len > rest)
            len = rest;
        size[blocks++] = len;
    }

    pick = (int)rnd(0, 2);
    pos = pick == 0 ? 0 : pick == 1 ? blocks : (int)rnd(0, blocks);
    for (i = blocks; i > pos; i--)
        size[i] = size[i - 1];
    size[pos] = w;
    blocks++;

    gap = small ? (int)rnd(1, 3) : 100000 / blocks;
    v = ends == 0 ? 0 : (int)rnd(0, gap - 1);
    for (i = 0; i < blocks; i++) {
        if (i > 0)
            v += (int)rnd(1, gap);
        vals[i] = v;
    }
    for (i = 0; i < blocks; i++)
        for (k = 0; k < size[i]; k++)
            arr[at++] = vals[i] + (ends == 1 ? 100000 - v : 0);

    put_ints(arr, n);
    free(arr);
    free(vals);
    free(size);
}
