/* Reference solution: count the digits, then deal them out from the
 * smallest up, alternating between the two numbers, so that the smallest
 * digits take the highest places and the largest the units. Leading zeros
 * cost nothing. O(number of digits) time. */
int splitNum(int num)
{
    int count[10] = {0}, a = 0, b = 0, turn = 0, d, i;

    while (num > 0) {
        count[num % 10]++;
        num /= 10;
    }
    for (d = 0; d <= 9; d++)
        for (i = 0; i < count[d]; i++) {
            if (turn == 0)
                a = a * 10 + d;
            else
                b = b * 10 + d;
            turn = !turn;
        }
    return a + b;
}
