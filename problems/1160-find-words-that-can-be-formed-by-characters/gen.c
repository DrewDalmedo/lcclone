/* Words tried against the letters of chars. Each word is made one of four
 * ways: letters taken from chars without reuse (it fits), a word that
 * needs exactly one more of some letter than chars has (it just fails), a
 * copy of an earlier word, or random letters. Small cases use the letters
 * a to d, up to 6 words of up to 6 letters and chars of up to 8, so that
 * letters repeat and both fits and near misses are common. Full-size cases
 * have 1000 words and chars of 100 letters, and the seed picks the shape:
 * every word a rearrangement of the whole of chars (the largest total,
 * 100000), every word a near miss of length 100, or a mix of all four. */
#define MAXL 100

static char words_buf[1000][MAXL + 1];

static void shuffle_letters(char *s, int len)
{
    int idx[MAXL], i;
    char tmp[MAXL];

    for (i = 0; i < len; i++)
        idx[i] = i;
    rnd_shuffle(idx, len);
    for (i = 0; i < len; i++)
        tmp[i] = s[idx[i]];
    memcpy(s, tmp, (size_t)len);
}

/* len letters of chars, each piece used at most once (len <= clen). */
static void fitting(const char *chars, int clen, char *w, int len)
{
    char tmp[MAXL];

    memcpy(tmp, chars, (size_t)clen);
    shuffle_letters(tmp, clen);
    memcpy(w, tmp, (size_t)len);
    w[len] = '\0';
}

/* A word that uses some letter once more than chars has it, plus other
 * pieces of chars: all of them that fit in maxlen when full is set, else a
 * random number. Falls back to a fitting word when no letter can be over
 * by one within maxlen (chars is a single letter repeated maxlen times). */
static void over_by_one(const char *chars, int clen, const char *alpha,
                        char *w, int maxlen, int full)
{
    char rest[MAXL], c = rnd_char(alpha);
    int k = 0, rlen = 0, len, extra, room, i;

    for (i = 0; i < clen; i++) {
        if (chars[i] == c)
            k++;
        else
            rest[rlen++] = chars[i];
    }
    if (k + 1 > maxlen) {
        fitting(chars, clen, w, clen < maxlen ? clen : maxlen);
        return;
    }
    for (i = 0; i <= k; i++)
        w[i] = c;
    len = k + 1;
    room = rlen < maxlen - len ? rlen : maxlen - len;
    extra = full ? room : (int)rnd(0, room);
    shuffle_letters(rest, rlen);
    memcpy(w + len, rest, (size_t)extra);
    len += extra;
    shuffle_letters(w, len);
    w[len] = '\0';
}

static void random_word(const char *alpha, char *w, int len)
{
    int i;

    for (i = 0; i < len; i++)
        w[i] = rnd_char(alpha);
    w[len] = '\0';
}

void gen(int small)
{
    const char *alpha;
    char chars[MAXL + 1], *ptrs[1000];
    int n, clen, maxlen, shape, kind, len, top, i;

    if (small) {
        alpha = rnd_chance(50) ? "abc" : "abcd";
        n = (int)rnd(1, 6);
        clen = (int)rnd(1, 8);
        maxlen = 6;
        shape = 2;
    } else {
        alpha = rnd_chance(50) ? "abcdefghijklmnopqrstuvwxyz" : "abcde";
        n = 1000;
        clen = MAXL;
        maxlen = MAXL;
        shape = (int)rnd(0, 2);
    }
    random_word(alpha, chars, clen);
    for (i = 0; i < n; i++) {
        ptrs[i] = words_buf[i];
        if (shape == 0) {
            fitting(chars, clen, ptrs[i], clen);
            continue;
        }
        if (shape == 1) {
            over_by_one(chars, clen, alpha, ptrs[i], maxlen, 1);
            continue;
        }
        kind = (int)rnd(0, 3);
        if (kind == 2 && i == 0)
            kind = 3;
        top = clen < maxlen ? clen : maxlen;
        if (kind == 0) {
            len = (int)rnd(1, top);
            fitting(chars, clen, ptrs[i], len);
        } else if (kind == 1) {
            over_by_one(chars, clen, alpha, ptrs[i], maxlen, 0);
        } else if (kind == 2) {
            len = (int)rnd(0, i - 1);
            strcpy(ptrs[i], words_buf[len]);
        } else {
            len = (int)rnd(1, maxlen);
            random_word(alpha, ptrs[i], len);
        }
    }
    put_strings(ptrs, n);
    put_string(chars);
}
