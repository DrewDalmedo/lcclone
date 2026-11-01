/* Slow and obviously right: work out every score, find the highest, then
 * look through divisors again for the smallest value with that score. */
int maxDivScore(int *nums, int numsSize, int *divisors, int divisorsSize)
{
    int *score = calloc((size_t)divisorsSize, sizeof *score);
    int top = 0, ans = 0, found = 0, i, j;

    for (i = 0; i < divisorsSize; i++)
        for (j = 0; j < numsSize; j++)
            if (nums[j] % divisors[i] == 0)
                score[i]++;
    for (i = 0; i < divisorsSize; i++)
        if (score[i] > top)
            top = score[i];
    for (i = 0; i < divisorsSize; i++)
        if (score[i] == top && (!found || divisors[i] < ans)) {
            ans = divisors[i];
            found = 1;
        }
    free(score);
    return ans;
}
