/* Reference solution: the odd numbers from 0 to x are (x + 1) / 2, so take
 * those up to high and remove those below low, of which there are
 * low / 2. high + 1 is at most 10^9 + 1, which fits an int. */
int countOdds(int low, int high)
{
    return (high + 1) / 2 - low / 2;
}
