/* Reference solution: count the cards of each number. The cards showing
 * one number fill whole piles of X exactly when X divides their count, so
 * a pile size X >= 2 exists exactly when the greatest common divisor of all
 * the counts is at least 2. */
static int gcd(int a, int b)
{
    int t;

    while (b) {
        t = a % b;
        a = b;
        b = t;
    }
    return a;
}

bool hasGroupsSizeX(int *deck, int deckSize)
{
    int *count = calloc(10000, sizeof *count);
    int g = 0, i;

    for (i = 0; i < deckSize; i++)
        count[deck[i]]++;
    for (i = 0; i < 10000; i++)
        if (count[i])
            g = gcd(g, count[i]);
    free(count);
    return g >= 2;
}
