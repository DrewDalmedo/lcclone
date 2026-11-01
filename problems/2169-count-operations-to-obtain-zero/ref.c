/* Reference solution: a run of moves that keep taking the smaller amount
 * out of the same larger one is a division, so count larger / smaller
 * moves at once and keep the remainder, as in Euclid's algorithm. Which
 * jar is which doesn't change the count. */
int countOperations(int num1, int num2)
{
    int count = 0, t;

    while (num1 > 0 && num2 > 0) {
        if (num1 < num2) {
            t = num1;
            num1 = num2;
            num2 = t;
        }
        count += num1 / num2;
        num1 %= num2;
    }
    return count;
}
