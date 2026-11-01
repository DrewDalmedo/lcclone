/* Slow and obviously right: for each query, try every subset of the
 * positions of nums (fine for the small random cases, n <= 10) and keep
 * the largest one whose sum stays within the query. */
int *answerQueries(int *nums, int numsSize, int *queries, int queriesSize,
                   int *returnSize)
{
    int *out = malloc((size_t)queriesSize * sizeof *out);
    long long sum;
    int i, k, mask, count;

    for (i = 0; i < queriesSize; i++) {
        out[i] = 0;
        for (mask = 0; mask < 1 << numsSize; mask++) {
            sum = 0;
            count = 0;
            for (k = 0; k < numsSize; k++)
                if (mask >> k & 1) {
                    sum += nums[k];
                    count++;
                }
            if (sum <= queries[i] && count > out[i])
                out[i] = count;
        }
    }
    *returnSize = queriesSize;
    return out;
}
