/* Reference solution: walk nums once. A run ends at the last element or
 * where the next element is not exactly one more (tested in long long, so
 * nothing overflows at the ends of the int range); each finished run is
 * printed with snprintf as "a" or "a->b". */
char **summaryRanges(int *nums, int numsSize, int *returnSize)
{
    char **out = malloc((size_t)(numsSize > 0 ? numsSize : 1) * sizeof *out);
    char buf[32];
    int m = 0, start = 0, i;

    for (i = 0; i < numsSize; i++) {
        if (i + 1 < numsSize && (long long)nums[i] + 1 == nums[i + 1])
            continue;
        if (start == i)
            snprintf(buf, sizeof buf, "%d", nums[i]);
        else
            snprintf(buf, sizeof buf, "%d->%d", nums[start], nums[i]);
        out[m] = malloc(strlen(buf) + 1);
        strcpy(out[m], buf);
        m++;
        start = i + 1;
    }
    *returnSize = m;
    return out;
}
