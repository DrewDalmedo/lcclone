/* A word over a few letters of the alphabet, each letter drawn in either
 * case, so that letters often occur in one case only, in both, or several
 * times. Small cases are up to 8 characters over at most three letters,
 * a, m and z among them; full-size ones are 50 characters long, over 1 to
 * 26 letters taken from anywhere in the alphabet. Some letters lean
 * heavily to one case, so a letter seen many times can still miss its
 * other form. */
void gen(int small)
{
    char pool[27], word[51];
    int lean[26];
    int n = small ? (int)rnd(1, 8) : 50;
    int k = small ? (int)rnd(1, 3) : (int)rnd(1, 26);
    int i, j, t, pick;

    for (i = 0; i < 26; i++)
        pool[i] = (char)('a' + i);
    if (small) {
        pool[1] = 'm';
        pool[2] = 'z';
    } else {
        for (i = 25; i > 0; i--) {
            j = (int)rnd(0, i);
            t = pool[i];
            pool[i] = pool[j];
            pool[j] = (char)t;
        }
    }
    for (i = 0; i < k; i++) {
        lean[i] = 50;
        if (rnd_chance(25)) {
            t = (int)rnd(0, 1);
            lean[i] = t ? 95 : 5;
        }
    }
    for (i = 0; i < n; i++) {
        pick = (int)rnd(0, k - 1);
        word[i] = pool[pick];
        if (rnd_chance(lean[pick]))
            word[i] = (char)(word[i] - 'a' + 'A');
    }
    word[n] = '\0';
    put_string(word);
}
