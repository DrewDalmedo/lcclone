/* A case is built from a vocabulary of distinct lowercase words. One word
 * that isn't banned is the winner and occurs c times; every other word
 * that isn't banned occurs fewer than c times, and banned words may occur
 * more often than the winner. The occurrences are shuffled and written out
 * with random capitals, one or more separators between words, and some
 * before the first word and after the last. Small cases use a few words
 * such as a, b, ab and ba, and up to 3 banned entries. Full-size cases are
 * close to 1000 characters; the seed picks hundreds of different one- and
 * two-letter words, up to 500 one-letter words with counts past 255, one
 * word of hundreds of letters, or words of up to 12 letters with 100
 * banned entries, some of them a letter short of or past a word. */
#define MAXV 702

static char vocab[MAXV][13];
static int len[MAXV], cnt[MAXV], cap[MAXV], ban[MAXV], v;
static int seq[500], nseq;
static char text[1001];
static int used; /* characters needed: the words and one separator each */

static char sep_char(void)
{
    return rnd_chance(60) ? ' ' : rnd_char("!?',;.");
}

static void put_word(const char *w, int n, char *out)
{
    int style = (int)rnd(0, 9), i;

    for (i = 0; i < n; i++) {
        out[i] = w[i];
        if ((style == 6 && i == 0) || style == 7 ||
            (style >= 8 && rnd_chance(50)))
            out[i] = (char)(w[i] - 'a' + 'A');
    }
}

/* Adds one occurrence of word i if its cap and the length allow it. */
static int add(int i)
{
    if (cnt[i] >= cap[i] || nseq == 500 || used + len[i] + 1 > 1001)
        return 0;
    cnt[i]++;
    seq[nseq++] = i;
    used += len[i] + 1;
    return 1;
}

static int find(const char *w)
{
    int i;

    for (i = 0; i < v; i++)
        if (strcmp(vocab[i], w) == 0)
            return i;
    return -1;
}

/* Writes the shuffled occurrences into text, with spare separators. */
static void write_text(void)
{
    int pos = 0, spare = 1001 - used, i, k, extra;

    rnd_shuffle(seq, nseq);
    extra = 0;
    if (spare > 0 && rnd_chance(50))
        extra = (int)rnd(1, spare < 2 ? spare : 2);
    for (k = 0; k < extra; k++)
        text[pos++] = sep_char();
    spare -= extra;
    for (i = 0; i < nseq; i++) {
        if (i > 0) {
            text[pos++] = sep_char();
            if (spare > 0 && rnd_chance(25)) {
                text[pos++] = sep_char();
                spare--;
            }
        }
        put_word(vocab[seq[i]], len[seq[i]], text + pos);
        pos += len[seq[i]];
    }
    extra = 0;
    if (spare > 0 && rnd_chance(50))
        extra = (int)rnd(1, spare < 2 ? spare : 2);
    for (k = 0; k < extra; k++)
        text[pos++] = sep_char();
    text[pos] = '\0';
}

static void random_word(char *w, int lo, int hi, const char *letters)
{
    int n = (int)rnd(lo, hi), i;

    for (i = 0; i < n; i++)
        w[i] = rnd_char(letters);
    w[n] = '\0';
}

static void long_word_case(char **bans, int *nb)
{
    static char banbuf[100][11];
    char longw[1001];
    int n, i, k, nshort = (int)rnd(0, 3), pos = 0, total;

    n = (int)rnd(11, 1000 - 4 * nshort);
    random_word(longw, n, n, "abcdefghijklmnopqrstuvwxyz");
    *nb = 0;
    for (k = 0; k < nshort; k++) {
        random_word(banbuf[*nb], 1, 3, "abc");
        bans[*nb] = banbuf[*nb];
        (*nb)++;
    }
    /* The short words, all banned, then the long word. */
    for (k = 0; k < nshort; k++) {
        put_word(bans[k], (int)strlen(bans[k]), text + pos);
        pos += (int)strlen(bans[k]);
        text[pos++] = sep_char();
    }
    total = pos + n;
    put_word(longw, n, text + pos);
    pos += n;
    if (total < 1000 && rnd_chance(50))
        text[pos++] = sep_char();
    text[pos] = '\0';
    for (i = 0; i < 3 && *nb < 100; i++) {
        /* A banned entry that starts the long word, but is shorter. */
        k = (int)rnd(1, 10);
        memcpy(banbuf[*nb], longw, (size_t)k);
        banbuf[*nb][k] = '\0';
        bans[*nb] = banbuf[*nb];
        (*nb)++;
    }
}

void gen(int small)
{
    static const char *const tiny[] = {"a", "b", "ab", "ba", "aa", "abc", "c"};
    static char banbuf[100][11];
    char *bans[100], w[13];
    int idx[MAXV], shape, winner, c, nb = 0, i, k, added, tries, pb;

    shape = small ? -1 : (int)rnd(0, 3);
    used = 0;
    nseq = 0;
    if (shape == 2) {
        long_word_case(bans, &nb);
        put_string(text);
        put_strings(bans, nb);
        return;
    }

    /* The vocabulary. */
    if (small) {
        v = (int)rnd(1, 5);
        for (i = 0; i < 7; i++)
            idx[i] = i;
        rnd_shuffle(idx, 7);
        for (i = 0; i < v; i++)
            strcpy(vocab[i], tiny[idx[i]]);
    } else if (shape == 0) {
        for (i = 0; i < MAXV; i++)
            idx[i] = i;
        rnd_shuffle(idx, MAXV);
        v = (int)rnd(120, 340);
        for (i = 0; i < v; i++) {
            k = idx[i];
            if (k < 26) {
                vocab[i][0] = (char)('a' + k);
                vocab[i][1] = '\0';
            } else {
                vocab[i][0] = (char)('a' + (k - 26) / 26);
                vocab[i][1] = (char)('a' + (k - 26) % 26);
                vocab[i][2] = '\0';
            }
        }
    } else if (shape == 1) {
        v = (int)rnd(2, 5);
        for (i = 0; i < 26; i++)
            idx[i] = i;
        rnd_shuffle(idx, 26);
        for (i = 0; i < v; i++) {
            vocab[i][0] = (char)('a' + idx[i]);
            vocab[i][1] = '\0';
        }
    } else {
        v = 0;
        k = (int)rnd(5, 40);
        for (tries = 0; tries < 200 && v < k; tries++) {
            random_word(w, 1, 12, "abcde");
            if (find(w) < 0)
                strcpy(vocab[v++], w);
        }
    }
    for (i = 0; i < v; i++)
        len[i] = (int)strlen(vocab[i]);

    /* The winner, the banned words and the caps on the counts. */
    winner = (int)rnd(0, v - 1);
    if (small)
        c = (int)rnd(1, 4);
    else if (shape == 0)
        c = (int)rnd(2, 3);
    else if (shape == 1)
        c = (int)rnd(100, 400);
    else
        c = (int)rnd(2, 15);
    pb = small ? 30 : shape == 0 ? 15 : 25;
    for (i = 0; i < v; i++) {
        cnt[i] = 0;
        ban[i] = i != winner && len[i] <= 10 && nb < (small ? 3 : 100) &&
                 rnd_chance(pb);
        if (ban[i]) {
            strcpy(banbuf[nb], vocab[i]);
            nb++;
        }
        cap[i] = ban[i] ? c + (int)rnd(0, 2) : i == winner ? c : c - 1;
    }

    /* Banned entries that never occur, some a letter short of or past a
     * word of the vocabulary, and some repeated. */
    k = small ? (int)rnd(0, 1) : shape == 3 ? 100 - nb : (int)rnd(0, 5);
    if (small && nb + k > 3)
        k = 3 - nb;
    for (i = 0; i < k && nb < 100; i++) {
        int from = (int)rnd(0, v - 1), r = (int)rnd(0, 3);

        strcpy(w, vocab[from]);
        if (r == 0 && len[from] > 1)
            w[len[from] - 1] = '\0';
        else if (r == 1 && len[from] < 10)
            strcat(w, small ? "b" : "e");
        else if (r == 2 && nb > 0)
            strcpy(w, banbuf[rnd(0, nb - 1)]);
        else
            random_word(w, 1, 10, small ? "abc" : "abcdefghijklmnopqrstuvwxyz");
        w[10] = '\0';
        if (find(w) >= 0 && !ban[find(w)])
            continue; /* would ban a word that is counted */
        strcpy(banbuf[nb], w);
        nb++;
    }
    for (i = 0; i < nb; i++)
        bans[i] = banbuf[i];
    for (i = 0; i < nb; i++) {
        char *t = bans[i];

        k = (int)rnd(i, nb - 1);
        bans[i] = bans[k];
        bans[k] = t;
    }

    /* The winner's occurrences, then rounds of one more of every word in
     * a random order, while the caps and the length allow. */
    for (i = 0; i < c; i++)
        add(winner);
    for (i = 0; i < v; i++)
        idx[i] = i;
    do {
        rnd_shuffle(idx, v);
        added = 0;
        for (i = 0; i < v; i++)
            added += add(idx[i]);
    } while (added > 0 && !(small && rnd_chance(40)));
    write_text();
    put_string(text);
    put_strings(bans, nb);
}
