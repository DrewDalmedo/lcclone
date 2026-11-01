/* Slow and obviously right: try every pair of turning points p < q and
 * check the three stretches straight from the definition. */
static bool rising(const int *a, int from, int to)
{
    int i;

    for (i = from; i < to; i++)
        if (a[i] >= a[i + 1])
            return false;
    return true;
}

static bool falling(const int *a, int from, int to)
{
    int i;

    for (i = from; i < to; i++)
        if (a[i] <= a[i + 1])
            return false;
    return true;
}

bool isTrionic(int *nums, int numsSize)
{
    int p, q;

    for (p = 1; p < numsSize - 1; p++)
        for (q = p + 1; q < numsSize - 1; q++)
            if (rising(nums, 0, p) && falling(nums, p, q) &&
                rising(nums, q, numsSize - 1))
                return true;
    return false;
}
