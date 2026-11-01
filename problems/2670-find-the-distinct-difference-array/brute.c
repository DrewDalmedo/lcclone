/* Slow and obviously right: for every cut, count each part's different
 * values from scratch, counting a value at its first place in that part,
 * where no earlier element of the part equals it. */
static int distinct(const int *a, int from, int to)
{
    int count = 0, i, j, first;

    for (i = from; i < to; i++) {
        first = 1;
        for (j = from; j < i; j++)
            if (a[j] == a[i])
                first = 0;
        count += first;
    }
    return count;
}

int *distinctDifferenceArray(int *nums, int numsSize, int *returnSize)
{
    int *answer = malloc((size_t)numsSize * sizeof *answer);
    int i;

    for (i = 0; i < numsSize; i++)
        answer[i] = distinct(nums, 0, i + 1) - distinct(nums, i + 1, numsSize);
    *returnSize = numsSize;
    return answer;
}
