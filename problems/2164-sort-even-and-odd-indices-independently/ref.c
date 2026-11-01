/* Reference solution: count how often each value occurs at even and at
 * odd indices (values are 1 to 100), then deal the even-index values back
 * from the smallest up and the odd-index values from the largest down. */
int *sortEvenOdd(int *nums, int numsSize, int *returnSize)
{
    int count[2][101] = {{0}};
    int *out = malloc((size_t)numsSize * sizeof *out);
    int i, v;

    for (i = 0; i < numsSize; i++)
        count[i % 2][nums[i]]++;
    v = 1;
    for (i = 0; i < numsSize; i += 2) {
        while (count[0][v] == 0)
            v++;
        out[i] = v;
        count[0][v]--;
    }
    v = 100;
    for (i = 1; i < numsSize; i += 2) {
        while (count[1][v] == 0)
            v--;
        out[i] = v;
        count[1][v]--;
    }
    *returnSize = numsSize;
    return out;
}
