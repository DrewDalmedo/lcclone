/* Slow and obviously right: build each concatenation as text, read it
 * back as a number, and remove elements from the array as described. */
long long findTheArrayConcVal(int *nums, int numsSize)
{
    int *a = malloc((size_t)numsSize * sizeof *a);
    int n = numsSize, i;
    long long total = 0;
    char buf[32];

    memcpy(a, nums, (size_t)n * sizeof *a);
    while (n > 0) {
        if (n == 1) {
            total += a[0];
            n = 0;
        } else {
            snprintf(buf, sizeof buf, "%d%d", a[0], a[n - 1]);
            total += strtoll(buf, NULL, 10);
            for (i = 0; i + 2 < n; i++)
                a[i] = a[i + 1];
            n -= 2;
        }
    }
    free(a);
    return total;
}
