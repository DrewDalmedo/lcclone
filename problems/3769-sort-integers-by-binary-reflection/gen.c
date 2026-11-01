/* Numbers built as (odd core) * 2^shift, so that elements of one family
 * share a reflection; plus some arbitrary values. The seed picks the
 * order: random, decreasing (so that sorting by reflection alone, keeping
 * the input order, puts a family the wrong way round) or increasing.
 * Small cases use cores up to 15 and shifts up to 4; full-size ones have
 * 100 elements up to 10^9, including 10^9, 2^29 and 805306367, whose
 * reflection 2^30 - 3 is above 10^9. */
static int cmp_desc(const void *pa, const void *pb)
{
    int a = *(const int *)pa, b = *(const int *)pb;

    return (a < b) - (a > b);
}

static int cmp_asc(const void *pa, const void *pb)
{
    int a = *(const int *)pa, b = *(const int *)pb;

    return (a > b) - (a < b);
}

static int family_member(int small)
{
    long long core, v;
    int shift, top_shift;

    if (small) {
        core = 2 * rnd(0, 7) + 1;
        shift = (int)rnd(0, 4);
        return (int)(core << shift);
    }
    core = 2 * rnd(0, 49) + 1;
    top_shift = 0;
    while ((core << (top_shift + 1)) <= 1000000000LL)
        top_shift++;
    shift = (int)rnd(0, top_shift);
    v = core << shift;
    return (int)v;
}

void gen(int small)
{
    static const int edges[] = {1000000000, 536870912, 805306367, 1,
                                999999999, 536870913};
    int n = small ? (int)rnd(1, 8) : 100, i, kind, order;
    int nums[100];

    for (i = 0; i < n; i++) {
        kind = (int)rnd(0, 9);
        if (kind < 7)
            nums[i] = family_member(small);
        else if (kind < 9)
            nums[i] = (int)rnd(1, small ? 40 : 1000000000);
        else
            nums[i] = small ? (int)rnd(1, 1000000000) : edges[rnd(0, 5)];
    }
    order = (int)rnd(0, 3);
    if (order == 1)
        qsort(nums, (size_t)n, sizeof *nums, cmp_desc);
    else if (order == 2)
        qsort(nums, (size_t)n, sizeof *nums, cmp_asc);
    put_ints(nums, n);
}
