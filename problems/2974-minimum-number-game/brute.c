/* Slow and obviously right: play the rounds. Each round finds the
 * smallest element not yet taken, then the smallest of the rest, marks
 * both as taken, and appends the second one, then the first. */
static int take_smallest(const int *nums, int n, bool *taken)
{
    int best = -1, i;

    for (i = 0; i < n; i++)
        if (!taken[i] && (best < 0 || nums[i] < nums[best]))
            best = i;
    taken[best] = true;
    return nums[best];
}

int *numberGame(int *nums, int numsSize, int *returnSize)
{
    int *arr = malloc((size_t)numsSize * sizeof *arr);
    bool taken[100] = {false};
    int m = 0, first, second;

    while (m < numsSize) {
        first = take_smallest(nums, numsSize, taken);
        second = take_smallest(nums, numsSize, taken);
        arr[m++] = second;
        arr[m++] = first;
    }
    *returnSize = numsSize;
    return arr;
}
