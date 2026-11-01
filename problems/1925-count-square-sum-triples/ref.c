/* Reference solution: for each c from 1 to n, look for legs a < b with
 * a^2 + b^2 = c^2 by moving two pointers towards each other, a from 1 up
 * and b from c - 1 down. Each pair found counts twice, for both orders of
 * the legs (a == b never works, as 2a^2 is not a square). O(n^2) time. */
int countTriples(int n)
{
    int count = 0, a, b, c, sum;

    for (c = 1; c <= n; c++) {
        a = 1;
        b = c - 1;
        while (a < b) {
            sum = a * a + b * b;
            if (sum == c * c) {
                count += 2;
                a++;
                b--;
            } else if (sum < c * c) {
                a++;
            } else {
                b--;
            }
        }
    }
    return count;
}
