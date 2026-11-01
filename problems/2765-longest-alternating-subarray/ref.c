/* Reference solution: one pass over the steps between neighbours, keeping
 * the length m of the seesaw that ends at the current element (0 if none).
 * The step that extends it must be +1 when m is odd and -1 when m is even;
 * failing that, a step of +1 still starts a new seesaw of length 2 at the
 * previous element. */
int alternatingSubarray(int *nums, int numsSize)
{
    int best = 0, m = 0, i, step;

    for (i = 1; i < numsSize; i++) {
        step = nums[i] - nums[i - 1];
        if (m >= 2 && step == (m % 2 == 1 ? 1 : -1))
            m++;
        else if (step == 1)
            m = 2;
        else
            m = 0;
        if (m > best)
            best = m;
    }
    return best > 0 ? best : -1;
}
