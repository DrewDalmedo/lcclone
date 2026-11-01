/* Different numbers from 1 to 100. Small cases: 3 to 8 numbers from a
 * narrow window of values (which sometimes touches 1 or 100), so that the
 * rows switch often. The seed also picks a shape: random, the first
 * number the smallest (everything goes to the bottom row), the second
 * number the smallest (everything goes to the top row), or falling
 * numbers (the rows take turns). Full-size cases have 50 numbers and the
 * same shapes. */
static int cmp_down(const void *a, const void *b)
{
    return *(const int *)b - *(const int *)a;
}

void gen(int small)
{
    int pool[100];
    int n = small ? (int)rnd(3, 8) : 50;
    int width = small ? (int)rnd(n, n + 6) : (int)rnd(n, 100);
    int where = (int)rnd(0, 3);
    int shape = (int)rnd(0, 6);
    int lo, i, low, tmp;

    if (where == 0)
        lo = 1;
    else if (where == 1)
        lo = 101 - width;
    else
        lo = (int)rnd(1, 101 - width);
    for (i = 0; i < width; i++)
        pool[i] = lo + i;
    rnd_shuffle(pool, width);
    if (shape == 1 || shape == 2) {
        /* Put the smallest of the n chosen numbers first or second. */
        low = 0;
        for (i = 1; i < n; i++)
            if (pool[i] < pool[low])
                low = i;
        tmp = pool[shape - 1];
        pool[shape - 1] = pool[low];
        pool[low] = tmp;
    } else if (shape == 3) {
        qsort(pool, (size_t)n, sizeof *pool, cmp_down);
    }
    put_ints(pool, n);
}
