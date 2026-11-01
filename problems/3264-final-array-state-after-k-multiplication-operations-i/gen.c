/* Small cases: 1 to 6 elements from 1 to 6, so that ties are common and
 * a multiplied element often meets or stays below the others. Full-size
 * ones: 100 elements, k = 10 and values from a range that may be narrow
 * (many ties) or the whole 1 to 100. multiplier is 1 now and then, so
 * that nothing changes. */
void gen(int small)
{
    int nums[100];
    int n = small ? (int)rnd(1, 6) : 100;
    int lo = small ? 1 : (int)rnd(1, 100), hi, k, multiplier, i;

    hi = small ? 6 : (int)rnd(lo, 100);
    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(lo, hi);
    k = small ? (int)rnd(1, 10) : 10;
    multiplier = rnd_chance(10) ? 1 : (int)rnd(2, 5);
    put_ints(nums, n);
    put_int(k);
    put_int(multiplier);
}
