/* Slow and obviously right: over every pair of positions i < j, check
 * whether the values never go down (nums[i] <= nums[j] for all of them)
 * and whether they never go up (nums[i] >= nums[j] for all of them). */
bool isMonotonic(int *nums, int numsSize)
{
    bool never_down = true, never_up = true;
    int i, j;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++) {
            if (nums[i] > nums[j])
                never_down = false;
            if (nums[i] < nums[j])
                never_up = false;
        }
    return never_down || never_up;
}
