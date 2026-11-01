/* Slow and obviously right: fill the envelopes one at a time with every
 * allowed amount (1 or more, never 4), keeping for each number of
 * envelopes filled and dollars used the most 8s seen; the answer is the
 * entry for all the envelopes and all the money. */
int distMoney(int money, int children)
{
    int best[31][201], j, s, a, v;

    for (j = 0; j <= children; j++)
        for (s = 0; s <= money; s++)
            best[j][s] = -1;
    best[0][0] = 0;
    for (j = 1; j <= children; j++)
        for (s = 0; s <= money; s++)
            for (a = 1; a <= s; a++) {
                if (a == 4 || best[j - 1][s - a] < 0)
                    continue;
                v = best[j - 1][s - a] + (a == 8);
                if (v > best[j][s])
                    best[j][s] = v;
            }
    return best[children][money];
}
