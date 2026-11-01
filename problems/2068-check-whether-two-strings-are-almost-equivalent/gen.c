/* Two words of the same length over a few letters (z among them at
 * times). The seed picks the shape: two unrelated words; or word2 made by
 * shuffling word1 and then turning 2 to 6 of its other letters into one
 * chosen letter, so that the largest gap lies right around the limit of
 * 3; or (full size only) word1 made of a single letter that word2 lacks.
 * Small cases are up to 16 letters long; full-size ones are 100. */
void gen(int small)
{
    static const char *alphabets[] = {
        "ab", "abz", "xyz", "abcd", "mnopqrstuvwxyz",
        "abcdefghijklmnopqrstuvwxyz",
    };
    char word1[101], word2[101], t, c;
    int n = small ? (int)rnd(1, 16) : 100, i, k, changes;
    int shape = (int)rnd(0, small ? 1 : 2);
    const char *letters = alphabets[rnd(0, small ? 3 : 5)];

    for (i = 0; i < n; i++)
        word1[i] = rnd_char(letters);
    if (shape == 0) {
        for (i = 0; i < n; i++)
            word2[i] = rnd_char(letters);
    } else {
        memcpy(word2, word1, (size_t)n);
        for (i = n - 1; i > 0; i--) {
            k = (int)rnd(0, i);
            t = word2[i];
            word2[i] = word2[k];
            word2[k] = t;
        }
        c = rnd_char(letters);
        changes = (int)rnd(2, 6);
        for (i = 0; i < n && changes > 0; i++)
            if (word2[i] != c) {
                word2[i] = c;
                changes--;
            }
        if (shape == 2) {
            memset(word1, 'z', (size_t)n);
            for (i = 0; i < n; i++)
                if (word2[i] == 'z')
                    word2[i] = 'a';
        }
    }
    word1[n] = word2[n] = '\0';
    put_string(word1);
    put_string(word2);
}
