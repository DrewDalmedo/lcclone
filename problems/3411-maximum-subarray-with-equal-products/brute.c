/* Slow and obviously right: for every start, grow the run one element at
 * a time, keeping its product, gcd and lcm, and compare them. gcd * lcm of
 * any run is at most 2520 (the lcm of 1..10), and the product only grows,
 * so once it passes 2520 no longer run from that start can be fair. */
static int gcd(int a, int b)
{
    while (b) {
        int t = a % b;

        a = b;
        b = t;
    }
    return a;
}

int maxLength(int *nums, int numsSize)
{
    int best = 0, i, j, g, l;
    long long prod;

    for (i = 0; i < numsSize; i++) {
        prod = 1;
        g = 0;
        l = 1;
        for (j = i; j < numsSize; j++) {
            prod *= nums[j];
            g = gcd(g, nums[j]);
            l = l / gcd(l, nums[j]) * nums[j];
            if (prod > 2520)
                break;
            if (prod == (long long)g * l && j - i + 1 > best)
                best = j - i + 1;
        }
    }
    return best;
}
