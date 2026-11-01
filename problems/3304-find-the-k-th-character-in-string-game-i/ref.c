/* Reference solution: count positions from 0, so position k is p = k - 1.
 * In a word of 2m letters, a position p >= m lies in the copy made by the
 * last round, and holds the letter one step after position p - m. Taking
 * off the highest bit of p in this way, one step per 1 bit, ends at
 * position 0, the "a". So the answer is "a" moved on once for every 1 bit
 * of k - 1. */
char kthCharacter(int k)
{
    int steps = 0, p;

    for (p = k - 1; p > 0; p >>= 1)
        steps += p & 1;
    return (char)('a' + steps % 26);
}
