/* Slow and obviously right: try every pile size X from 2 to the size of the
 * deck. X works when, for every card, the number of cards showing the same
 * number (counted by scanning the whole deck) is a multiple of X, since
 * those cards must fill whole piles of X. */
bool hasGroupsSizeX(int *deck, int deckSize)
{
    int x, i, j, same, works;

    for (x = 2; x <= deckSize; x++) {
        works = 1;
        for (i = 0; i < deckSize && works; i++) {
            same = 0;
            for (j = 0; j < deckSize; j++)
                if (deck[j] == deck[i])
                    same++;
            if (same % x != 0)
                works = 0;
        }
        if (works)
            return true;
    }
    return false;
}
