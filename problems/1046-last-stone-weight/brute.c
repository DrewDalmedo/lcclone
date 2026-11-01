/* Slow and obviously right: follow the rules on a copy of the stones,
 * sorting the whole copy again before every step so that the two
 * heaviest are at the end. */
static int ascending(const void *x, const void *y)
{
    int a = *(const int *)x, b = *(const int *)y;

    return (a > b) - (a < b);
}

int lastStoneWeight(int *stones, int stonesSize)
{
    int pile[30], n = stonesSize, i, a, b;

    for (i = 0; i < n; i++)
        pile[i] = stones[i];
    while (n >= 2) {
        qsort(pile, (size_t)n, sizeof *pile, ascending);
        a = pile[n - 1];
        b = pile[n - 2];
        n -= 2;
        if (a != b)
            pile[n++] = a - b;
    }
    return n == 1 ? pile[0] : 0;
}
