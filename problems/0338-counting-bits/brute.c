/* Slow and obviously right: write each number from 0 to n in base 2 by
 * dividing by 2 over and over, and count the remainders that are 1. */
int *countBits(int n, int *returnSize)
{
    int *ans = malloc((size_t)(n + 1) * sizeof *ans);
    int i, x, count;

    for (i = 0; i <= n; i++) {
        count = 0;
        for (x = i; x > 0; x /= 2)
            if (x % 2 == 1)
                count++;
        ans[i] = count;
    }
    *returnSize = n + 1;
    return ans;
}
