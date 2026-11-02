/* Slow and obviously right: for every index, check each later index and
 * count the pairs where exactly one of the two values is even. */
int *countOppositeParity(int *nums, int numsSize, int *returnSize)
{
    int *answer = malloc((size_t)numsSize * sizeof *answer);
    int i, j, even_i, even_j;

    for (i = 0; i < numsSize; i++) {
        answer[i] = 0;
        even_i = nums[i] % 2 == 0;
        for (j = i + 1; j < numsSize; j++) {
            even_j = nums[j] % 2 == 0;
            if (even_i != even_j)
                answer[i]++;
        }
    }
    *returnSize = numsSize;
    return answer;
}
