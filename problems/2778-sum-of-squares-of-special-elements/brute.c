/* Slow and obviously right: element number p (counting from 1) is special
 * when some whole number q gives p * q == n; try every q. */
int sumOfSquares(int *nums, int numsSize)
{
    int n = numsSize, sum = 0, p, q;

    for (p = 1; p <= n; p++)
        for (q = 1; q <= n; q++)
            if (p * q == n) {
                sum += nums[p - 1] * nums[p - 1];
                break;
            }
    return sum;
}
