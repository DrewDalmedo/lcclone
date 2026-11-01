/* Reference solution: a flush when every suit matches the first one;
 * otherwise count the ranks in a table of 14 slots and name the hand by the
 * largest count. */
char *bestHand(int *ranks, int ranksSize, char *suits, int suitsSize)
{
    int count[14] = {0}, most = 0, i;
    bool flush = true;
    const char *name;
    char *out;

    for (i = 1; i < suitsSize; i++)
        if (suits[i] != suits[0])
            flush = false;
    for (i = 0; i < ranksSize; i++)
        if (++count[ranks[i]] > most)
            most = count[ranks[i]];
    if (flush)
        name = "Flush";
    else if (most >= 3)
        name = "Three of a Kind";
    else if (most == 2)
        name = "Pair";
    else
        name = "High Card";
    out = malloc(strlen(name) + 1);
    strcpy(out, name);
    return out;
}
