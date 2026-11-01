/* Slow and obviously right: for c = 0, 1, 2, ..., look at every group of k
 * adjacent cells anew and stop at the first c for which some group holds
 * no more than c white cells. */
int minimumRecolors(char *blocks, int k)
{
    int n = (int)strlen(blocks), c, start, i, white;

    for (c = 0;; c++)
        for (start = 0; start + k <= n; start++) {
            white = 0;
            for (i = start; i < start + k; i++)
                if (blocks[i] == 'W')
                    white++;
            if (white <= c)
                return c;
        }
}
