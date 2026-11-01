/* Two strings, usually copies of one short word so that a stamp exists.
 * The seed picks the shape: copies of one word; the same with one letter
 * changed (often the very last one); str2 made of copies of a rotation of
 * the word; random letters; or copies of one word with one string cut
 * short in the middle of its last copy. Small cases use the letters A and B, words
 * of 1 to 3 letters and strings of at most 12; full-size ones use words of
 * up to 10 letters, one string as long as the copies allow (up to 1000
 * letters), and one shape gives one letter 1000 and 999 times. */
static int copies_into(char *s, const char *w, int wl, int copies)
{
    int i;

    for (i = 0; i < wl * copies; i++)
        s[i] = w[i % wl];
    s[wl * copies] = '\0';
    return wl * copies;
}

static char other_letter(char c, const char *letters)
{
    int k = (int)strlen(letters), at = (int)(strchr(letters, c) - letters);
    int step = (int)rnd(1, k - 1);

    return letters[(at + step) % k];
}

void gen(int small)
{
    const char *letters = small ? "AB" : (rnd_chance(50) ? "ABC" : "XYZWV");
    int maxlen = small ? 12 : 1000;
    int shape = (int)rnd(0, small ? 4 : 5);
    char word[11], rot[11], s1[1001], s2[1001], *s;
    int wl = (int)rnd(1, small ? 3 : 10), c1, c2, l1, l2, r, i, p;

    for (i = 0; i < wl; i++)
        word[i] = rnd_char(letters);
    c1 = small ? (int)rnd(1, maxlen / wl) : maxlen / wl;
    c2 = (int)rnd(1, maxlen / wl);
    if (rnd_chance(50)) { /* either string may be the longer one */
        i = c1;
        c1 = c2;
        c2 = i;
    }
    l1 = copies_into(s1, word, wl, c1);
    l2 = copies_into(s2, word, wl, c2);
    switch (shape) {
    case 0: /* copies of one word */
        break;
    case 1: /* one letter changed, often the last */
        s = rnd_chance(50) ? s1 : s2;
        p = (int)strlen(s) - 1;
        if (rnd_chance(50))
            p = (int)rnd(0, p);
        s[p] = other_letter(s[p], letters);
        break;
    case 2: /* str2 from a rotation of the word */
        r = wl > 1 ? (int)rnd(1, wl - 1) : 0;
        for (i = 0; i < wl; i++)
            rot[i] = word[(i + r) % wl];
        l2 = copies_into(s2, rot, wl, c2);
        break;
    case 3: /* random letters */
        l1 = (int)rnd(1, maxlen);
        l2 = (int)rnd(1, maxlen);
        for (i = 0; i < l1; i++)
            s1[i] = rnd_char(letters);
        for (i = 0; i < l2; i++)
            s2[i] = rnd_char(letters);
        s1[l1] = '\0';
        s2[l2] = '\0';
        break;
    case 4: /* one string stops partway through its last copy */
        s = rnd_chance(50) ? s1 : s2;
        p = (int)strlen(s);
        r = wl > 1 ? (int)rnd(1, wl - 1) : 0;
        if (p > r)
            s[p - r] = '\0';
        break;
    default: /* one letter, 1000 and 999 times, either way round */
        l1 = rnd_chance(50) ? 1000 : 999;
        copies_into(s1, word, 1, l1);
        copies_into(s2, word, 1, 1999 - l1);
    }
    put_string(s1);
    put_string(s2);
}
