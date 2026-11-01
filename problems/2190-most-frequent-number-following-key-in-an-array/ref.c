/* Reference solution: one pass over the neighbouring pairs. A table
 * indexed by value (1 to 1000) counts how often each value comes right
 * after an entry equal to key; the value whose count is largest so far is
 * kept as the answer. */
int mostFrequent(int *nums, int numsSize, int key)
{
    int count[1001] = {0};
    int best = 0, i, v;

    for (i = 0; i + 1 < numsSize; i++)
        if (nums[i] == key) {
            v = nums[i + 1];
            if (++count[v] > count[best])
                best = v;
        }
    return best;
}
