/* Reference solution: one pass, counting the ones since the last 0 and
 * keeping the largest count seen. The best is updated at every 1, so a
 * streak that runs to the end of the array counts too. */
int findMaxConsecutiveOnes(int *nums, int numsSize)
{
    int run = 0, best = 0, i;

    for (i = 0; i < numsSize; i++) {
        if (nums[i] == 1) {
            run++;
            if (run > best)
                best = run;
        } else {
            run = 0;
        }
    }
    return best;
}
