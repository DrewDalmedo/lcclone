/* Reference solution: turning the leftmost 6 into a 9 gains the most, and
 * with no 6 nothing can be gained. Walk the digits from the right, keep
 * the place value of the last 6 seen (the leftmost one), and add 3 times
 * it. */
int maximum69Number(int num)
{
    int place = 1, top = 0, x;

    for (x = num; x > 0; x /= 10, place *= 10)
        if (x % 10 == 6)
            top = place;
    return num + 3 * top;
}
