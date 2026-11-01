/* Slow and obviously right: go through every index, and keep the smallest
 * distance from start among those that hold target. */
int getMinDistance(int *nums, int numsSize, int target, int start)
{
    int best = numsSize, i, dist;

    for (i = 0; i < numsSize; i++)
        if (nums[i] == target) {
            dist = i > start ? i - start : start - i;
            if (dist < best)
                best = dist;
        }
    return best;
}
