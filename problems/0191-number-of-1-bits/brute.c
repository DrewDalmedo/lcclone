/* Slow and obviously right: write n in base 2 by dividing by 2 over and
 * over, and count the remainders that are 1. */
int hammingWeight(int n)
{
    int count = 0;

    while (n > 0) {
        if (n % 2 == 1)
            count++;
        n /= 2;
    }
    return count;
}
