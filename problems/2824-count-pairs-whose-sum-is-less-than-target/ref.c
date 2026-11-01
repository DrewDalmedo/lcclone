/* Reference solution: sort a copy, then move two pointers inwards. When
 * the values at lo and hi add up to less than target, so does lo with each
 * of lo+1..hi, which counts hi - lo pairs at once; otherwise hi is too
 * large to pair with anything still left, so it moves down. */
static int by_value(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int countPairs(int *nums, int numsSize, int target)
{
    int *v = malloc((size_t)numsSize * sizeof *v);
    int lo = 0, hi = numsSize - 1, count = 0;

    memcpy(v, nums, (size_t)numsSize * sizeof *v);
    qsort(v, (size_t)numsSize, sizeof *v, by_value);
    while (lo < hi) {
        if (v[lo] + v[hi] < target) {
            count += hi - lo;
            lo++;
        } else {
            hi--;
        }
    }
    free(v);
    return count;
}
