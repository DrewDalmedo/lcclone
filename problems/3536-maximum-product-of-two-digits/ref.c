/* Reference solution: no digit is negative, so the best product is the
 * largest digit times the second largest, which is the largest again
 * when it appears twice. Peel the digits off with % 10, keeping the top
 * two; n has at least two digits, so both get set. */
int maxProduct(int n)
{
    int first = -1, second = -1, d;

    while (n > 0) {
        d = n % 10;
        n /= 10;
        if (d > first) {
            second = first;
            first = d;
        } else if (d > second) {
            second = d;
        }
    }
    return first * second;
}
