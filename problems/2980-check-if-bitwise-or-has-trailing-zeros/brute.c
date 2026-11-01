/* Slow and obviously right: try every choice of two or more elements
 * (every subset, as a bit mask), OR them together, and count the trailing
 * zeros of the result. gen.c's small cases have at most 8 elements; for
 * more than 20, this falls back to trying every pair, which is enough:
 * when a choice works, so does any two of its elements, since a 0 bit in
 * an OR is a 0 bit in each element. */
static int trailing_zeros(int v)
{
    int count = 0;

    while (v % 2 == 0) {
        v /= 2;
        count++;
    }
    return count;
}

bool hasTrailingZeros(int *nums, int numsSize)
{
    int n = numsSize, i, k, size, acc;
    long mask;

    if (n > 20) {
        for (i = 0; i < n; i++)
            for (k = i + 1; k < n; k++)
                if (trailing_zeros(nums[i] | nums[k]) >= 1)
                    return true;
        return false;
    }
    for (mask = 1; mask < 1L << n; mask++) {
        size = 0;
        acc = 0;
        for (i = 0; i < n; i++)
            if (mask >> i & 1) {
                acc |= nums[i];
                size++;
            }
        if (size >= 2 && trailing_zeros(acc) >= 1)
            return true;
    }
    return false;
}
