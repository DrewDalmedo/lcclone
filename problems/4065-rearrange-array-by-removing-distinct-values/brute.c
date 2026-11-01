/* Slow and obviously right: run the rounds as described. Each round walks
 * over what is left of nums, takes out the first copy of every value it
 * has not taken yet in this round, sorts those values and appends them. */
int *rearrangeArray(int *nums, int numsSize, int *returnSize)
{
    int *left = malloc((size_t)numsSize * sizeof *left);
    int *ans = malloc((size_t)numsSize * sizeof *ans);
    int round[100], nleft = numsSize, len = 0, taken, keep, i, j, seen, t;

    for (i = 0; i < numsSize; i++)
        left[i] = nums[i];
    while (nleft > 0) {
        taken = 0;
        keep = 0;
        for (i = 0; i < nleft; i++) {
            seen = 0;
            for (j = 0; j < taken; j++)
                if (round[j] == left[i])
                    seen = 1;
            if (seen)
                left[keep++] = left[i];
            else
                round[taken++] = left[i];
        }
        nleft = keep;
        for (i = 1; i < taken; i++)
            for (j = i; j > 0 && round[j - 1] > round[j]; j--) {
                t = round[j];
                round[j] = round[j - 1];
                round[j - 1] = t;
            }
        for (i = 0; i < taken; i++)
            ans[len++] = round[i];
    }
    free(left);
    *returnSize = len;
    return ans;
}
