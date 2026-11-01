/* Small cases use up to 8 letters from a tiny alphabet, so repeats are
 * common. magazine is usually the note's letters shuffled, with some
 * spare letters, and then often one copy of a letter the note needs is
 * taken out, so that exactly that letter is one short. Otherwise
 * magazine's letters are all in the note with more besides (the roles
 * swapped), or the two strings are unrelated. Full-size cases have
 * strings of up to 10^5 letters, in a shape the seed picks: magazine a
 * shuffle of the note; a note of 5 * 10^4 z's against a magazine that
 * has its z's at the far end; a shuffle with one copy of the note's last
 * letter swapped for another letter; a note of 5 * 10^4 letters and a
 * magazine twice as long; or two unrelated strings. */
static const char letters[] = "abcdefghijklmnopqrstuvwxyz";

static void shuffle(char *a, int n)
{
    int i, k;
    char c;

    for (i = n - 1; i > 0; i--) {
        k = (int)rnd(0, i);
        c = a[i];
        a[i] = a[k];
        a[k] = c;
    }
}

/* A letter of abc other than c (abc has at least two letters). */
static char other_than(const char *abc, char c)
{
    char d;

    do
        d = rnd_char(abc);
    while (d == c);
    return d;
}

static void random_letters(char *a, int n, const char *abc)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] = rnd_char(abc);
}

static void small_case(char *note, char *mag, int *n, int *m)
{
    static const char *const alphabets[] = {"ab", "abc", "az", "xyz", letters};
    const char *abc = alphabets[rnd(0, 4)];
    int shape = (int)rnd(0, 7), i, k, e;
    char c;

    *n = (int)rnd(1, 6);
    random_letters(note, *n, abc);
    if (shape <= 2) {
        /* every letter there, with up to 8 - n spares */
        e = (int)rnd(0, 8 - *n);
        memcpy(mag, note, (size_t)*n);
        random_letters(mag + *n, e, abc);
        *m = *n + e;
        shuffle(mag, *m);
    } else if (shape <= 4) {
        /* one copy of c short; the spares are never c */
        k = (int)rnd(0, *n - 1);
        c = note[k];
        *m = 0;
        for (i = 0; i < *n; i++)
            if (i != k)
                mag[(*m)++] = note[i];
        e = (int)rnd(*m == 0 ? 1 : 0, 8 - *n);
        for (i = 0; i < e; i++)
            mag[(*m)++] = other_than(abc, c);
        shuffle(mag, *m);
    } else if (shape == 5) {
        /* the note is magazine with more letters */
        *m = (int)rnd(1, 7);
        random_letters(mag, *m, abc);
        e = (int)rnd(1, 8 - *m);
        memcpy(note, mag, (size_t)*m);
        random_letters(note + *m, e, abc);
        *n = *m + e;
        shuffle(note, *n);
    } else {
        *m = (int)rnd(1, 8);
        random_letters(mag, *m, abc);
    }
}

static void large_case(char *note, char *mag, int *n, int *m)
{
    int shape = (int)(gen_seed() % 5), i;
    char c;

    if (shape == 1) {
        *n = 50000;
        *m = 100000;
        memset(note, 'z', (size_t)*n);
        random_letters(mag, *m - *n, "abcdefghijklmnopqrstuvwxy");
        memset(mag + (*m - *n), 'z', (size_t)*n);
    } else if (shape == 3) {
        *n = 50000;
        *m = 100000;
        random_letters(note, *n, letters);
        memcpy(mag, note, (size_t)*n);
        random_letters(mag + *n, *m - *n, letters);
        shuffle(mag, *m);
    } else if (shape == 4) {
        *n = *m = 100000;
        random_letters(note, *n, letters);
        random_letters(mag, *m, letters);
    } else {
        *n = *m = 100000;
        random_letters(note, *n, letters);
        memcpy(mag, note, (size_t)*n);
        shuffle(mag, *m);
        if (shape == 2) {
            /* note[n - 1] is the last time the note needs c */
            c = note[*n - 1];
            for (i = 0; mag[i] != c; i++)
                ;
            mag[i] = other_than(letters, c);
        }
    }
}

void gen(int small)
{
    char *note = malloc(100001), *mag = malloc(100001);
    int n, m;

    if (small)
        small_case(note, mag, &n, &m);
    else
        large_case(note, mag, &n, &m);
    note[n] = '\0';
    mag[m] = '\0';
    put_string(note);
    put_string(mag);
    free(note);
    free(mag);
}
