/* nums: distinct values in increasing order, picked from a range by
 * selection sampling. Small cases take up to 8 values from [-12, 12], and
 * target is often one of them, else anything in or just around that
 * range. Full-size cases take 10^4 values from [-10^4, 10^4], and the seed
 * picks target: past the last element (the slowest for a scan from the
 * front), the last element, below the first, a random element, or any
 * value in the range. */
void gen(int small)
{
    int lo = small ? -12 : -10000, hi = small ? 12 : 10000;
    int n = small ? (int)rnd(1, 8) : 10000;
    int shape = small ? 9 : (int)rnd(0, 4);
    int *nums = malloc((size_t)n * sizeof *nums);
    int need = n, left, v, k = 0, target;

    if (shape == 0)
        hi--; /* room for a target past the end */
    else if (shape == 2)
        lo++; /* room for a target below the start */
    left = hi - lo + 1;
    for (v = lo; v <= hi && need > 0; v++, left--)
        if (rnd(0, left - 1) < need) {
            nums[k++] = v;
            need--;
        }
    switch (shape) {
    case 0:
        target = 10000;
        break;
    case 1:
        target = nums[n - 1];
        break;
    case 2:
        target = -10000;
        break;
    case 3:
        target = nums[rnd(0, n - 1)];
        break;
    case 4:
        target = (int)rnd(-10000, 10000);
        break;
    default:
        target = rnd_chance(40) ? nums[rnd(0, n - 1)]
                                : (int)rnd(lo - 3, hi + 3);
    }
    put_ints(nums, n);
    put_int(target);
    free(nums);
}
