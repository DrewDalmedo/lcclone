/* Reference solution: add up the digits of a copy of x, so that x itself
 * is still there for the remainder test. */
int sumOfTheDigitsOfHarshadNumber(int x)
{
    int rest = x, s = 0;

    while (rest > 0) {
        s += rest % 10;
        rest /= 10;
    }
    return x % s == 0 ? s : -1;
}
