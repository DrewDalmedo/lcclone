/* Sentences built as word chains: most words begin with the letter the
 * word before ended with, and the last word often ends with the first
 * word's first letter, so true and false both come up. Small cases are
 * 1 to 4 words of 1 to 3 letters from a, b and A. Full-size cases fill up
 * to 500 characters, in a shape the seed picks: one long word, a chain of
 * short words, 250 one-letter words, or words of random letters. A chain
 * may then get one link broken, often only by the case of a letter. */
static char toggle(char c)
{
    return c >= 'a' && c <= 'z' ? (char)(c - 'a' + 'A') : (char)(c - 'A' + 'a');
}

void gen(int small)
{
    char s[501];
    int lens[250], starts[250];
    const char *letters = small ? "abA" : "abcxyzABC";
    int words = 0, len = 0, shape, chain, close, i, k, wl, p;
    char first = 'a', last = 'a', c;

    shape = small ? 1 : (int)rnd(0, 3);
    if (shape == 0) {
        lens[words++] = 500;
    } else if (shape == 2) {
        while (words < 250)
            lens[words++] = 1;
    } else {
        int count = small ? (int)rnd(1, 4) : 250;
        int top = small ? 3 : (int)rnd(2, 12);

        while (words < count) {
            wl = (int)rnd(1, top);
            if (len + (words > 0) + wl > 500)
                break;
            len += (words > 0) + wl;
            lens[words++] = wl;
        }
    }
    chain = shape != 3 && rnd_chance(80);
    close = rnd_chance(70);
    len = 0;
    for (i = 0; i < words; i++) {
        if (i > 0)
            s[len++] = ' ';
        starts[i] = len;
        for (k = 0; k < lens[i]; k++) {
            if (k == 0 && i > 0 && (chain || shape == 2))
                c = last;
            else if (k == lens[i] - 1 && k > 0 && i == words - 1 && close)
                c = first;
            else
                c = rnd_char(letters);
            s[len++] = c;
            if (i == 0 && k == 0)
                first = c;
        }
        last = s[len - 1];
    }
    s[len] = '\0';
    /* Maybe break one link of a chain: change the first letter of a word
     * other than the first, or the last letter of the sentence. */
    if (chain && rnd_chance(40)) {
        p = words > 1 ? (int)rnd(0, words - 1) : 0;
        if (p == 0)
            p = len - 1;
        else
            p = starts[p];
        c = rnd_chance(50) ? toggle(s[p]) : rnd_char(letters);
        s[p] = c;
    }
    put_string(s);
}
