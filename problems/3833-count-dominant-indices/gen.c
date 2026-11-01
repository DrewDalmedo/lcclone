/* Random arrays. Small ones use values 1 to 6 or fewer, so that ties with
 * an exact average come up often. Full-size ones have 100 elements, the
 * seed picking the shape: values anywhere in 1 to 100, values in a narrow
 * band of four (many ties), or random values sorted in decreasing order,
 * where nearly every index counts and runs of equal values make ties. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100, i, j, t, shape, lo, hi;
    int nums[100];

    if (small) {
        hi = (int)rnd(1, 6);
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, hi);
        put_ints(nums, n);
        return;
    }
    shape = (int)rnd(0, 2);
    lo = shape == 1 ? (int)rnd(1, 97) : 1;
    hi = shape == 1 ? lo + 3 : 100;
    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(lo, hi);
    if (shape == 2)
        for (i = 0; i < n; i++)
            for (j = i + 1; j < n; j++)
                if (nums[j] > nums[i]) {
                    t = nums[i];
                    nums[i] = nums[j];
                    nums[j] = t;
                }
    put_ints(nums, n);
}
