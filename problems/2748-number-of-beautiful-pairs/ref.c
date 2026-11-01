/* Reference solution: walk nums once, keeping how many earlier numbers
 * start with each digit 1..9. A number, as the later one of a pair, makes
 * a beautiful pair with every earlier number whose leading digit is
 * coprime with its own units digit. */
static int gcd(int a, int b)
{
    int t;

    while (b != 0) {
        t = a % b;
        a = b;
        b = t;
    }
    return a;
}

int countBeautifulPairs(int *nums, int numsSize)
{
    int lead_count[10] = {0};
    int pairs = 0, i, d, lead;

    for (i = 0; i < numsSize; i++) {
        for (d = 1; d <= 9; d++)
            if (gcd(d, nums[i] % 10) == 1)
                pairs += lead_count[d];
        lead = nums[i];
        while (lead >= 10)
            lead /= 10;
        lead_count[lead]++;
    }
    return pairs;
}
