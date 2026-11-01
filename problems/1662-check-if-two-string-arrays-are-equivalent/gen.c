/* A text s, cut into the strings of word1, and a second text cut into the
 * strings of word2. Each cut puts a break between two letters with a
 * chance picked per array (none, some, or every letter on its own). The
 * second text is, by the seed's choice: s itself, cut again elsewhere
 * (true); s with one letter changed; s one letter shorter or longer (a
 * prefix of the other); or word1's strings in reverse order. Half of the
 * time the two arrays swap places, so either side can be the shorter one.
 * Small texts have 1 to 8 letters from "ab" or "abc"; full-size ones have
 * 1000 letters, and change the last letter when they change one. */
struct cut {
    char *piece[1000];
    int count;
    char buf[2001];
};

static struct cut c1, c2;

/* Cuts s[0..len) into pieces, breaking between two letters with a chance
 * of pct in 100. */
static void cut(struct cut *c, const char *s, int len, int pct)
{
    int at = 0, i;

    c->count = 0;
    c->piece[c->count++] = c->buf;
    for (i = 0; i < len; i++) {
        if (i > 0 && rnd_chance(pct)) {
            c->buf[at++] = '\0';
            c->piece[c->count++] = c->buf + at;
        }
        c->buf[at++] = s[i];
    }
    c->buf[at] = '\0';
}

static int pick_pct(void)
{
    int r = (int)rnd(0, 3);

    if (r == 0)
        return 0;
    if (r == 1)
        return 100;
    return (int)rnd(1, 60);
}

void gen(int small)
{
    const char *alpha = small ? (rnd_chance(70) ? "ab" : "abc")
                              : (rnd_chance(50) ? "ab"
                                                : "abcdefghijklmnopqrstuvwxyz");
    int m = (int)strlen(alpha), len = small ? (int)rnd(1, 8) : 1000;
    int shape = (int)rnd(0, small ? 4 : 5), len2 = len, i, pos, pct, which;
    char s[1001], t[1002];

    for (i = 0; i < len; i++)
        s[i] = rnd_char(alpha);
    memcpy(t, s, (size_t)len);
    if (shape == 2 || shape == 5) {
        /* One letter changed: anywhere, or (shape 5) the last one. */
        pos = shape == 5 ? len - 1 : (int)rnd(0, len - 1);
        which = (int)(strchr(alpha, s[pos]) - alpha);
        which = (which + (int)rnd(1, m - 1)) % m;
        t[pos] = alpha[which];
    } else if (shape == 3) {
        /* One letter shorter, or one longer when that keeps to the limits. */
        if (len > 1 && (!small || rnd_chance(50))) {
            len2 = len - 1;
        } else {
            t[len] = rnd_char(alpha);
            len2 = len + 1;
        }
    }
    pct = pick_pct();
    cut(&c1, s, len, pct);
    if (shape == 4) {
        c2.count = c1.count;
        for (i = 0; i < c1.count; i++)
            c2.piece[i] = c1.piece[c1.count - 1 - i];
    } else {
        pct = pick_pct();
        cut(&c2, t, len2, pct);
    }
    if (rnd_chance(50)) {
        put_strings(c1.piece, c1.count);
        put_strings(c2.piece, c2.count);
    } else {
        put_strings(c2.piece, c2.count);
        put_strings(c1.piece, c1.count);
    }
}
