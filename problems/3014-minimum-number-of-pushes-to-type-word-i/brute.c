/* Slow and obviously right: as every letter of word occurs once, a key
 * that carries c of them costs 1 + 2 + ... + c presses, whichever letters
 * they are. So a labelling's cost depends only on how many letters each
 * of the eight keys gets. Try every way of sharing the m letters among
 * the keys (in non-increasing shares, since which key is which doesn't
 * matter) and keep the cheapest. */
static int cheapest(int keys, int letters, int most)
{
    int best = INT_MAX, c, rest;

    if (keys == 0)
        return letters == 0 ? 0 : INT_MAX;
    for (c = 0; c <= most && c <= letters; c++) {
        rest = cheapest(keys - 1, letters - c, c);
        if (rest != INT_MAX && c * (c + 1) / 2 + rest < best)
            best = c * (c + 1) / 2 + rest;
    }
    return best;
}

int minimumPushes(char *word)
{
    int m = (int)strlen(word);

    return cheapest(8, m, m);
}
