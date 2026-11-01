/* Reference solution: an earlier index i pairs with index j exactly when
 * i is a multiple of k / gcd(j, k). Walk j from left to right, keeping, for
 * every value and every divisor d of k, how many earlier indices holding
 * that value are multiples of d. That is O(n * divisors(k)) time. */
static int gcd(int a, int b)
{
    int t;

    while (b) {
        t = a % b;
        a = b;
        b = t;
    }
    return a;
}

int countPairs(int *nums, int numsSize, int k)
{
    int cnt[101][101];
    int count = 0, j, d, v;

    memset(cnt, 0, sizeof cnt);
    for (j = 0; j < numsSize; j++) {
        v = nums[j];
        count += cnt[v][k / gcd(j, k)];
        for (d = 1; d <= k; d++)
            if (k % d == 0 && j % d == 0)
                cnt[v][d]++;
    }
    return count;
}
