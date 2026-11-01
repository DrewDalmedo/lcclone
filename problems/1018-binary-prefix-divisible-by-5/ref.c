/* Reference solution: only the counter's remainder modulo 5 matters, and
 * doubling and adding a bit acts on remainders the same way, so keep just
 * the remainder, from 0 to 4. */
bool *prefixesDivBy5(int *nums, int numsSize, int *returnSize)
{
    bool *answer = malloc((size_t)numsSize * sizeof *answer);
    int rem = 0, i;

    for (i = 0; i < numsSize; i++) {
        rem = (rem * 2 + nums[i]) % 5;
        answer[i] = rem == 0;
    }
    *returnSize = numsSize;
    return answer;
}
