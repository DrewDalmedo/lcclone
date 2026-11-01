/* Slow and obviously right: try the labels in order, straight from their
 * definitions: compare every two suits for a flush, then try every three
 * cards for a shared rank, then every two cards. */
char *bestHand(int *ranks, int ranksSize, char *suits, int suitsSize)
{
    const char *name = "High Card";
    bool same = true, three = false, two = false;
    int i, j, k;
    char *out;

    for (i = 0; i < suitsSize; i++)
        for (j = 0; j < suitsSize; j++)
            if (suits[i] != suits[j])
                same = false;
    for (i = 0; i < ranksSize; i++)
        for (j = i + 1; j < ranksSize; j++) {
            if (ranks[i] == ranks[j])
                two = true;
            for (k = j + 1; k < ranksSize; k++)
                if (ranks[i] == ranks[j] && ranks[j] == ranks[k])
                    three = true;
        }
    if (same)
        name = "Flush";
    else if (three)
        name = "Three of a Kind";
    else if (two)
        name = "Pair";
    out = malloc(strlen(name) + 1);
    strcpy(out, name);
    return out;
}
