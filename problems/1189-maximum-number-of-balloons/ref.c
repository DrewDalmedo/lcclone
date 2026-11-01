/* Reference solution: count each letter, then see which of b, a, l, o
 * and n runs out first, halving the counts of l and o since every copy
 * of the word needs two of each. */
int maxNumberOfBalloons(char *text)
{
    int count[26] = {0}, best;
    const char *p;

    for (p = text; *p; p++)
        count[*p - 'a']++;
    best = count['b' - 'a'];
    if (count['a' - 'a'] < best)
        best = count['a' - 'a'];
    if (count['l' - 'a'] / 2 < best)
        best = count['l' - 'a'] / 2;
    if (count['o' - 'a'] / 2 < best)
        best = count['o' - 'a'] / 2;
    if (count['n' - 'a'] < best)
        best = count['n' - 'a'];
    return best;
}
