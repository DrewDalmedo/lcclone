/* The seed picks the shape: random letters, or k letters with a common
 * count c changed in one way (one letter one copy more, one letter two
 * more, one letter one fewer, two letters one more each, one extra letter
 * with a single copy, or no change), shuffled into a random order, so that
 * both answers come up often. Small cases have up to 4 letters with counts
 * up to 3 (at most 14 characters); full-size ones use up to 26 letters and
 * reach 100 characters, or come close. */
void gen(int small)
{
    int lt[26], count[26] = {0}, pos[100], shape = (int)rnd(0, 6);
    int n = 0, k, c, cmax, i, j;
    char word[101];

    for (i = 0; i < 26; i++)
        lt[i] = i;
    rnd_shuffle(lt, 26);
    if (shape == 0) {
        k = small ? (int)rnd(1, 4) : (int)rnd(1, 26);
        n = small ? (int)rnd(2, 8) : 100;
        for (i = 0; i < n; i++)
            pos[i] = lt[rnd(0, k - 1)];
    } else {
        k = small ? (int)rnd(1, 4) : (int)rnd(1, 26);
        cmax = small ? 3 : 98 / k;
        c = rnd_chance(small ? 0 : 60) ? cmax : (int)rnd(1, cmax);
        for (i = 0; i < k; i++)
            count[i] = c;
        if (shape == 2 && k == 26)
            shape = 1;
        if (shape == 5 && c == 1)
            shape = 1;
        if (shape == 6 && k == 1)
            shape = 3;
        if (shape == 1)
            count[0]++;
        else if (shape == 2)
            count[k] = 1;
        else if (shape == 3)
            count[0] += 2;
        else if (shape == 5)
            count[0]--;
        else if (shape == 6) {
            count[0]++;
            count[1]++;
        }
        if (k == 1 && count[0] + count[1] < 2)
            count[0] = 2;
        for (i = 0; i < 26; i++)
            for (j = 0; j < count[i]; j++)
                pos[n++] = lt[i];
        rnd_shuffle(pos, n);
    }
    for (i = 0; i < n; i++)
        word[i] = (char)('a' + pos[i]);
    word[n] = '\0';
    put_string(word);
}
