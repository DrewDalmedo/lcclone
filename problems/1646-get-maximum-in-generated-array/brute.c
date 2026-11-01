/* Slow and obviously right: work out every a[j] afresh by recursing on
 * the definition, with no table, and take the largest. */
static int value(int j)
{
    if (j < 2)
        return j;
    if (j % 2 == 0)
        return value(j / 2);
    return value((j - 1) / 2) + value((j + 1) / 2);
}

int getMaximumGenerated(int n)
{
    int best = 0, j, v;

    for (j = 0; j <= n; j++) {
        v = value(j);
        if (v > best)
            best = v;
    }
    return best;
}
