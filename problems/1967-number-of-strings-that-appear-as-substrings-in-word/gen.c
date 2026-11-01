/* A word, and patterns cut from it (some of them then changed by a
 * letter, or made one letter longer), random ones, and copies of earlier
 * patterns. Small cases: a word of up to 8 letters of a, b and c, and up
 * to 6 patterns. Full-size cases have a word of 100 letters and 100
 * patterns, in one of two shapes picked by the seed: random letters, or
 * a word of a's with perhaps one b somewhere against patterns of a's with
 * a b last, the slowest shape for a letter-by-letter search. */
static char *piece(const char *from, int len)
{
    char *p = malloc((size_t)len + 2);

    memcpy(p, from, (size_t)len);
    p[len] = '\0';
    return p;
}

void gen(int small)
{
    const char *letters = small ? "abc" : "ab";
    int w = small ? (int)rnd(1, 8) : 100;
    int m = small ? (int)rnd(1, 6) : 100;
    int repeated = !small && rnd_chance(50);
    int shape, len, at, i, j;
    char word[101], tmp[101], **patterns = malloc((size_t)m * sizeof *patterns), c;

    for (j = 0; j < w; j++)
        word[j] = repeated ? 'a' : rnd_char(letters);
    if (repeated && rnd_chance(70)) {
        at = (int)rnd(0, w - 1);
        word[at] = 'b';
    }
    word[w] = '\0';
    for (i = 0; i < m; i++) {
        shape = (int)rnd(0, 4);
        if (repeated) {
            len = (int)rnd(1, 100);
            memset(tmp, 'a', (size_t)len);
            if (shape > 0)
                tmp[len - 1] = 'b';
            patterns[i] = piece(tmp, len);
        } else if (shape == 4 && i > 0) {   /* a copy of an earlier one */
            at = (int)rnd(0, i - 1);
            patterns[i] = piece(patterns[at], (int)strlen(patterns[at]));
        } else if (shape == 3) {            /* random, maybe too long */
            len = small ? (int)rnd(1, 10) : (int)rnd(1, 100);
            for (j = 0; j < len; j++)
                tmp[j] = rnd_char(letters);
            patterns[i] = piece(tmp, len);
        } else {                            /* cut from word */
            len = (int)rnd(1, w);
            at = (int)rnd(0, w - len);
            patterns[i] = piece(word + at, len);
            if (shape == 1) {               /* one letter changed */
                at = (int)rnd(0, len - 1);
                do
                    c = rnd_char(letters);
                while (c == patterns[i][at]);
                patterns[i][at] = c;
            } else if (shape == 2 && len < 100) { /* one letter more */
                patterns[i][len] = rnd_char(letters);
                patterns[i][len + 1] = '\0';
            }
        }
    }
    put_strings(patterns, m);
    put_string(word);
    for (i = 0; i < m; i++)
        free(patterns[i]);
    free(patterns);
}
