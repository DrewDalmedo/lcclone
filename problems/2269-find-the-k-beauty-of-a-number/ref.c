/* Reference solution: arithmetic on num, no string. rest starts as num and
 * loses its last digit at each step; while it still has at least k
 * digits, its last k digits (rest % 10^k) are the next frame, from right
 * to left. 10^k reaches 10^10 for num = 10^9 and k = 10, so it is a long
 * long. */
int divisorSubstrings(int num, int k)
{
    long long mod = 1, rest;
    int count = 0, v, i;

    for (i = 0; i < k; i++)
        mod *= 10;
    for (rest = num; rest >= mod / 10; rest /= 10) {
        v = (int)(rest % mod);
        if (v != 0 && num % v == 0)
            count++;
    }
    return count;
}
