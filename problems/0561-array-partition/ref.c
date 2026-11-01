/* Reference solution: in sorted order the best pairing matches neighbours,
 * so the answer is the sum of the elements at even positions of the sorted
 * array. A counting sort over the 2 * 10^4 + 1 possible values gets there
 * in linear time. "take" says whether the next element in sorted order
 * sits at an even position; it carries on from one value to the next. */
int arrayPairSum(int *nums, int numsSize)
{
    int *count = calloc(20001, sizeof *count);
    int i, v, take = 1, sum = 0;

    for (i = 0; i < numsSize; i++)
        count[nums[i] + 10000]++;
    for (v = 0; v <= 20000; v++)
        for (i = 0; i < count[v]; i++) {
            if (take)
                sum += v - 10000;
            take = !take;
        }
    free(count);
    return sum;
}
