/* Slow and obviously right: for each -1, count the -1 entries in a row
 * that end there to get t, then walk back through nums from that point,
 * skipping -1 entries, to the t-th positive entry. */
int *lastVisitedIntegers(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int answers = 0, i, j, t, seen, answer;

    for (i = 0; i < numsSize; i++) {
        if (nums[i] != -1)
            continue;
        t = 0;
        for (j = i; j >= 0 && nums[j] == -1; j--)
            t++;
        answer = -1;
        seen = 0;
        for (j = i; j >= 0; j--)
            if (nums[j] != -1 && ++seen == t) {
                answer = nums[j];
                break;
            }
        out[answers++] = answer;
    }
    *returnSize = answers;
    return out;
}
