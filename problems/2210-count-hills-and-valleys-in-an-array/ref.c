/* Reference solution: one pass that skips over equal neighbours. An
 * element is judged only when the next one differs, so each plateau once,
 * at its last element; prev, the height of the plateau before it, moves
 * on only then. (The first plateau compares equal to prev and so never
 * counts.) */
int countHillValley(int *nums, int numsSize)
{
    int prev = nums[0], count = 0, i;

    for (i = 1; i + 1 < numsSize; i++) {
        if (nums[i] == nums[i + 1])
            continue;
        if ((nums[i] > prev && nums[i] > nums[i + 1])
            || (nums[i] < prev && nums[i] < nums[i + 1]))
            count++;
        prev = nums[i];
    }
    return count;
}
