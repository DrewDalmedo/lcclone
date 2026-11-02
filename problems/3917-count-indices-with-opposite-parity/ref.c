/* Reference solution: walk from the right end, counting the even and the
 * odd values passed so far; each entry's answer is the count of the other
 * parity, taken before the entry itself is counted. */
int *countOppositeParity(int *nums, int numsSize, int *returnSize)
{
    int *answer = malloc((size_t)numsSize * sizeof *answer);
    int seen[2] = {0, 0}, i, p;

    for (i = numsSize - 1; i >= 0; i--) {
        p = nums[i] % 2;
        answer[i] = seen[1 - p];
        seen[p]++;
    }
    *returnSize = numsSize;
    return answer;
}
