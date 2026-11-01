/* Small cases have 1 to 6 chunks over the letters a and b, each either a
 * valid word of a random shape (letters, letters-letters, a lone mark,
 * either kind of stem with a mark) or 1 to 4 random characters of
 * "ab-.!,1", so that misplaced hyphens and marks and stray digits come up
 * often; between chunks are 1 to 3 spaces, and sometimes spaces at the
 * ends. Full-size cases are up to 1000 characters, in a shape the seed
 * picks: 500 one-character chunks, mostly valid; a random mix of chunks
 * and gaps; one chunk of 1000 characters, valid or with one flaw; or only
 * valid words. */
static const char letters[] = "abcdefghijklmnopqrstuvwxyz";

static int put_letters(char *t, int n, const char *pool)
{
    int i;

    for (i = 0; i < n; i++)
        t[i] = rnd_char(pool);
    return n;
}

/* Writes a valid word of a random shape at t and returns its length. */
static int valid_word(char *t, int small)
{
    const char *pool = small ? "ab" : letters;
    int top = small ? 2 : 6, shape = (int)rnd(0, 4), len = 0, k;

    if (shape != 2) {
        k = (int)rnd(1, top);
        len += put_letters(t + len, k, pool);
        if (shape == 1 || shape == 4) {
            t[len++] = '-';
            k = (int)rnd(1, top);
            len += put_letters(t + len, k, pool);
        }
    }
    if (shape >= 2)
        t[len++] = rnd_char("!.,");
    return len;
}

/* Writes a chunk at t, valid or random, and returns its length. */
static int chunk(char *t, int small)
{
    int k;

    if (rnd_chance(50))
        return valid_word(t, small);
    if (small) {
        k = (int)rnd(1, 4);
        return put_letters(t, k, "ab-.!,1");
    }
    k = (int)rnd(1, 8);
    return put_letters(t, k, "abcdefghijklmnopqrstuvwxyz--..!!,,0123456789");
}

static int spaces(char *s, int n)
{
    int i;

    for (i = 0; i < n; i++)
        s[i] = ' ';
    return n;
}

void gen(int small)
{
    char s[1001], t[32];
    int len = 0, ntok, i, k, gap, tlen, shape, lead, trail, where;

    if (small) {
        ntok = (int)rnd(1, 6);
        lead = rnd_chance(25) ? (int)rnd(1, 2) : 0;
        len += spaces(s + len, lead);
        for (i = 0; i < ntok; i++) {
            if (i > 0) {
                gap = rnd_chance(60) ? 1 : (int)rnd(2, 3);
                len += spaces(s + len, gap);
            }
            len += chunk(s + len, 1);
        }
        trail = rnd_chance(25) ? (int)rnd(1, 2) : 0;
        len += spaces(s + len, trail);
    } else {
        shape = (int)(gen_seed() % 4);
        if (shape == 0) {
            /* 500 one-character chunks, a space before or after each:
             * 1000 characters, with a space at one end. */
            k = (int)rnd(0, 1);
            for (i = 0; i < 1000; i++)
                s[len++] = i % 2 == k ? rnd_char("abcxyz!.,-7") : ' ';
        } else if (shape == 2) {
            /* One chunk: 1000 letters with a hyphen and maybe a mark. */
            len = put_letters(s, 1000, letters);
            where = (int)rnd(1, 998);
            s[where] = '-';
            if (rnd_chance(50))
                s[999] = rnd_char("!.,");
            if (rnd_chance(50)) {
                /* One flaw at a random place: a digit, a second hyphen
                 * or a mark that is not last. */
                where = (int)rnd(0, 998);
                s[where] = rnd_char("5--.!");
            }
        } else {
            /* Chunks until 1000 characters, separated by gaps of 1 to 3
             * spaces (single spaces for the all-valid shape). */
            lead = shape == 1 ? (int)rnd(0, 3) : 0;
            len += spaces(s + len, lead);
            for (i = 0;; i++) {
                tlen = shape == 1 ? chunk(t, 0) : valid_word(t, 0);
                gap = i == 0 ? 0 : shape == 1 ? (int)rnd(1, 3) : 1;
                if (len + gap + tlen > 1000)
                    break;
                len += spaces(s + len, gap);
                memcpy(s + len, t, (size_t)tlen);
                len += tlen;
            }
            if (shape == 1) {
                trail = (int)rnd(0, 1000 - len);
                len += spaces(s + len, trail);
            }
        }
    }
    s[len] = '\0';
    put_string(s);
}
