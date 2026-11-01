/* Reference solution: fix the left end i and stretch the slice to the
 * right one element at a time, keeping a table of the values seen since i
 * and how many of them there are; add the square after each step. */
int sumCounts(int *nums, int numsSize)
{
    int seen[101];
    int total = 0, i, j, count;

    for (i = 0; i < numsSize; i++) {
        memset(seen, 0, sizeof seen);
        count = 0;
        for (j = i; j < numsSize; j++) {
            if (!seen[nums[j]]) {
                seen[nums[j]] = 1;
                count++;
            }
            total += count * count;
        }
    }
    return total;
}
