/* Reference solution: a match can only be 1 to n (x = 0 never matches,
 * as all n >= 1 elements are at least 0, and no more than n elements are
 * at least anything). Count
 * the elements by value, putting every value above n in bucket n; then,
 * going down from x = n, the count of elements >= x is a running sum of
 * the buckets. */
int specialArray(int *nums, int numsSize)
{
    int *bucket = calloc((size_t)numsSize + 1, sizeof *bucket);
    int atLeast = 0, x, i, answer = -1;

    for (i = 0; i < numsSize; i++)
        bucket[nums[i] < numsSize ? nums[i] : numsSize]++;
    for (x = numsSize; x >= 1; x--) {
        atLeast += bucket[x];
        if (atLeast == x) {
            answer = x;
            break;
        }
    }
    free(bucket);
    return answer;
}
