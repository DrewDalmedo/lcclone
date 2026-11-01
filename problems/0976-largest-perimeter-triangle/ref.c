/* Reference solution: sort the lengths from longest to shortest. For a
 * longest side d[i], the best partners are the next two lengths, d[i + 1]
 * and d[i + 2]; if even they are too short, no triangle has d[i] as its
 * longest side. So the first i where d[i + 1] + d[i + 2] > d[i] gives the
 * answer. (Lengths up to 10^6 keep every sum well inside an int.) */
static int cmp_desc(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x < y) - (x > y);
}

int largestPerimeter(int *nums, int numsSize)
{
    int i;

    qsort(nums, (size_t)numsSize, sizeof *nums, cmp_desc);
    for (i = 0; i + 2 < numsSize; i++)
        if (nums[i + 1] + nums[i + 2] > nums[i])
            return nums[i] + nums[i + 1] + nums[i + 2];
    return 0;
}
