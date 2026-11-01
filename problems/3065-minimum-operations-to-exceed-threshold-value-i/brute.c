/* Slow and obviously right: act out the sorter on a copy of the crate.
 * Find the lightest apple; if it weighs less than k, take that one apple
 * out and count a step, and look again. */
int minOperations(int *nums, int numsSize, int k)
{
    int crate[50], len = numsSize, steps = 0, i, light;

    for (i = 0; i < len; i++)
        crate[i] = nums[i];
    while (len > 0) {
        light = 0;
        for (i = 1; i < len; i++)
            if (crate[i] < crate[light])
                light = i;
        if (crate[light] >= k)
            break;
        for (i = light + 1; i < len; i++)
            crate[i - 1] = crate[i];
        len--;
        steps++;
    }
    return steps;
}
