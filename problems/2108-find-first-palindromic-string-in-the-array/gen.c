/* Words are built as palindromes, then left alone, spoiled at one pair of
 * letters, or redrawn at random. Small cases are 1 to 6 words of up to 5
 * letters from "ab" or "abc", so that palindromes, near misses (the
 * outermost or the innermost pair wrong) and repeated words come up
 * often. Full-size ones are 100 words of 100 letters, each spoiled only at
 * its innermost pair, so that telling it from a palindrome takes a full
 * pass; the one real palindrome is last, at a random place, or nowhere,
 * as the seed picks. */
static char text[100][101];

/* Fills w with a random palindrome of len letters. */
static void mirror(char *w, int len, const char *letters)
{
    int i;
    char c;

    for (i = 0; i < (len + 1) / 2; i++) {
        c = rnd_char(letters);
        w[i] = c;
        w[len - 1 - i] = c;
    }
    w[len] = '\0';
}

/* Makes the letters p places from each end differ (p < len / 2). */
static void spoil(char *w, int len, int p)
{
    w[p] = w[len - 1 - p] == 'a' ? 'b' : 'a';
}

void gen(int small)
{
    static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    char *words[100];
    const char *letters = rnd_chance(50) ? "ab" : "abc";
    int m, len, i, k, kind, pal;

    if (small) {
        m = (int)rnd(1, 6);
        for (i = 0; i < m; i++) {
            len = (int)rnd(1, 5);
            mirror(text[i], len, letters);
            kind = (int)rnd(0, 2);
            if (kind == 1 && len >= 2) {
                k = (int)rnd(0, len / 2 - 1);
                spoil(text[i], len, k);
            } else if (kind == 2) {
                for (k = 0; k < len; k++)
                    text[i][k] = rnd_char(letters);
            }
            words[i] = text[i];
        }
    } else {
        m = 100;
        for (i = 0; i < m; i++) {
            mirror(text[i], 100, alphabet);
            spoil(text[i], 100, 49);
            words[i] = text[i];
        }
        switch (gen_seed() % 3) {
        case 0:
            pal = m - 1;
            break;
        case 1:
            pal = (int)rnd(0, m - 1);
            break;
        default:
            pal = -1;
            break;
        }
        if (pal >= 0)
            mirror(text[pal], 100, alphabet);
    }
    put_strings(words, m);
}
