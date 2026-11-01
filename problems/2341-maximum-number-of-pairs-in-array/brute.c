/* Slow and obviously right: draw pairs one at a time, each time searching
 * the marbles still in the jar for two that show the same number, until no
 * such two are left; then count the marbles left. */
int *numberOfPairs(int *nums, int numsSize, int *returnSize)
{
    bool gone[100] = {false};
    int *out = malloc(2 * sizeof *out);
    int pairs = 0, left = 0, found = 1, i, j;

    while (found) {
        found = 0;
        for (i = 0; i < numsSize && !found; i++)
            for (j = i + 1; j < numsSize && !found; j++)
                if (!gone[i] && !gone[j] && nums[i] == nums[j]) {
                    gone[i] = gone[j] = true;
                    pairs++;
                    found = 1;
                }
    }
    for (i = 0; i < numsSize; i++)
        if (!gone[i])
            left++;
    out[0] = pairs;
    out[1] = left;
    *returnSize = 2;
    return out;
}
