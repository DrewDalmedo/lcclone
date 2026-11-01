/* The letters of a case come from a pool: a few letters for small cases,
 * so that near misses and ties of length are common, and 2 to 26 letters
 * for full-size ones. The plate mixes pool letters of either case with
 * digits and spaces. Words are random, near misses (every needed letter
 * but one, padded with other letters) or completing words. Full-size cases
 * have 1000 words of up to 15 letters, nearly all near misses; the seed
 * picks whether only the last word completes the plate, completing words
 * are scattered through the list, or every word has 15 letters. A case
 * with no completing word gets one at a random place. */
static int need[26], nneed;

static int completes(const char *w)
{
    int have[26] = {0}, i;

    for (; *w; w++)
        have[*w - 'a']++;
    for (i = 0; i < 26; i++)
        if (have[i] < need[i])
            return 0;
    return 1;
}

static void shuffle(char *s, int n)
{
    int i, k;
    char t;

    for (i = n - 1; i > 0; i--) {
        k = (int)rnd(0, i);
        t = s[i];
        s[i] = s[k];
        s[k] = t;
    }
}

/* Writes to w a word of about len letters: the needed letters, with one
 * fewer of the letter drop (none fewer when drop is -1), then other pool
 * letters, never drop, in a random order. Returns 0 when no such word of
 * 1 to 15 letters exists. */
static int make(char *w, int len, const char *pool, int drop)
{
    char others[27];
    int n = 0, no = 0, i, k;

    for (i = 0; pool[i]; i++)
        if (pool[i] - 'a' != drop)
            others[no++] = pool[i];
    others[no] = '\0';
    for (i = 0; i < 26; i++)
        for (k = 0; k < need[i] - (i == drop); k++)
            w[n++] = (char)('a' + i);
    if (no == 0)
        len = n;
    if (len < n)
        len = n;
    if (len > 15)
        len = 15;
    if (len < 1 || n > len) {
        w[0] = '\0';
        return 0;
    }
    while (n < len)
        w[n++] = rnd_char(others);
    w[n] = '\0';
    shuffle(w, n);
    return 1;
}

/* A near miss of about len letters, or failing that a random word. */
static void near_miss(char *w, int len, const char *pool)
{
    int drop = -1, k, i, which;

    if (nneed > 0) {
        which = (int)rnd(0, nneed - 1);
        for (i = 0; i < 26; i++) {
            if (which < need[i]) {
                drop = i;
                break;
            }
            which -= need[i];
        }
        if (make(w, len, pool, drop))
            return;
    }
    if (len < 1)
        len = 1;
    for (k = 0; k < len; k++)
        w[k] = rnd_char(pool);
    w[len] = '\0';
}

void gen(int small)
{
    static char words[1000][16];
    char *ptrs[1000], pool[27], plate[8], c;
    int letters[26], a, p, n, i, k, r, len, shape, found;

    a = small ? (int)rnd(1, 4) : (int)rnd(2, 26);
    for (i = 0; i < 26; i++)
        letters[i] = i;
    rnd_shuffle(letters, 26);
    for (i = 0; i < a; i++)
        pool[i] = (char)('a' + letters[i]);
    pool[a] = '\0';

    p = small ? (int)rnd(1, 7) : 7;
    memset(need, 0, sizeof need);
    nneed = 0;
    for (i = 0; i < p; i++) {
        r = (int)rnd(0, 9);
        if (r < (small ? 6 : 8)) {
            c = rnd_char(pool);
            need[c - 'a']++;
            nneed++;
            if (rnd_chance(40))
                c = (char)(c - 'a' + 'A');
        } else if (r < 9) {
            c = rnd_char("0123456789");
        } else {
            c = ' ';
        }
        plate[i] = c;
    }
    plate[p] = '\0';

    n = small ? (int)rnd(1, 8) : 1000;
    shape = (int)rnd(0, 2);
    for (k = 0; k < n; k++) {
        ptrs[k] = words[k];
        words[k][0] = '\0';
        if (small) {
            r = (int)rnd(0, 9);
            len = nneed + (int)rnd(-1, 2);
            if (k > 0 && r == 0)
                strcpy(words[k], words[rnd(0, k - 1)]);
            else if (r < 4)
                near_miss(words[k], len, pool);
            else if (r < 7)
                make(words[k], len, pool, -1);
            else
                near_miss(words[k], (int)rnd(1, 6), pool);
            if (words[k][0] == '\0')
                near_miss(words[k], 1, pool);
            continue;
        }
        len = shape == 2 ? 15 : (int)rnd(nneed > 1 ? nneed - 1 : 1, 15);
        if (shape == 0 && k == n - 1)
            make(words[k], len, pool, -1);
        else if (shape != 0 && rnd_chance(4))
            make(words[k], len, pool, -1);
        else
            near_miss(words[k], len, pool);
    }

    found = 0;
    for (k = 0; k < n; k++)
        if (completes(words[k]))
            found = 1;
    if (!found) {
        k = (int)rnd(0, n - 1);
        len = small ? nneed + (int)rnd(0, 2) : (int)rnd(nneed, 15);
        make(words[k], len, pool, -1);
    }
    put_string(plate);
    put_strings(ptrs, n);
}
