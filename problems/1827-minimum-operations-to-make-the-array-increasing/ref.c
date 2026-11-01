/* Reference solution (greedy): go left to right, keeping the final height
 * of the previous stack. Each stack must end up at least one above it, and
 * raising it any higher would only cost more, now and for the stacks after
 * it; so a stack that is too low goes to exactly previous + 1. */
int minOperations(int *nums, int numsSize)
{
    int moves = 0, prev = nums[0], i;

    for (i = 1; i < numsSize; i++) {
        if (nums[i] > prev) {
            prev = nums[i];
        } else {
            moves += prev + 1 - nums[i];
            prev++;
        }
    }
    return moves;
}
