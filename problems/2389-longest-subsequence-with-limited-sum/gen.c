/* Small cases have 1 to 8 tasks of 1 to 5 or 1 to 20 minutes and 1 to 6
 * budgets. Full-size cases have 1000 tasks and 1000 budgets, in a shape
 * the seed picks: tasks of up to 10^6 minutes; tasks of up to 2000
 * minutes, so that the answers spread from 0 to 1000; tasks of 1 or 2
 * minutes; or the longest tasks first. Budgets are often the exact total
 * of the c shortest tasks, or one less, where an off-by-one in the search
 * shows, or at least the total of all tasks, so that every task fits;
 * the rest are random. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

static int cmp_desc(const void *a, const void *b)
{
    return cmp_int(b, a);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 1000, m = small ? (int)rnd(1, 6) : 1000;
    int *nums = malloc((size_t)n * sizeof *nums);
    int *queries = malloc((size_t)m * sizeof *queries);
    int *sorted = malloc((size_t)n * sizeof *sorted);
    long long *p = malloc((size_t)(n + 1) * sizeof *p), top, q;
    int shape = small ? 0 : (int)(gen_seed() % 4), i, c, kind;

    if (small)
        top = rnd_chance(50) ? 5 : 20;
    else
        top = shape == 0 || shape == 3 ? 1000000 : shape == 1 ? 2000 : 2;
    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(1, top);
    if (shape == 3)
        qsort(nums, (size_t)n, sizeof *nums, cmp_desc);
    memcpy(sorted, nums, (size_t)n * sizeof *nums);
    qsort(sorted, (size_t)n, sizeof *sorted, cmp_int);
    p[0] = 0;
    for (i = 0; i < n; i++)
        p[i + 1] = p[i] + sorted[i];
    for (i = 0; i < m; i++) {
        kind = (int)rnd(0, 4);
        c = (int)rnd(0, n);
        if (kind <= 1)
            q = p[c];
        else if (kind == 2)
            q = p[c] - 1;
        else if (kind == 3 || p[n] > 1000000)
            q = rnd(1, p[n] + 10 < 1000000 ? p[n] + 10 : 1000000);
        else
            q = rnd(p[n], small ? p[n] + 10 : 1000000);
        if (q < 1)
            q = 1;
        if (q > 1000000)
            q = rnd(1, 1000000);
        queries[i] = (int)q;
    }
    put_ints(nums, n);
    put_ints(queries, m);
    free(nums);
    free(queries);
    free(sorted);
    free(p);
}
