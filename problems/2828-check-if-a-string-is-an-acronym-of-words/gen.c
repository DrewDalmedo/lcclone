/* Words of a and b (a to e at full size), and an s made from their
 * initials: for a third of the seeds exactly the initials, else the
 * initials one letter short, one letter longer, with one letter changed,
 * with two neighbours exchanged, or the last letters of the words instead;
 * sometimes a random string. Small cases have up to 6 words of 1 to 3
 * letters. Full-size ones have 99 or 100 words of up to 10 letters, so s
 * reaches 100 letters, and a changed letter sits near the end. */
void gen(int small)
{
    const char *letters = small ? "ab" : "abcde";
    int m = small ? (int)rnd(1, 6) : (int)rnd(99, 100);
    int shape = (int)rnd(0, 8), n, len, i, j, at;
    char **words = malloc((size_t)m * sizeof *words);
    char s[102], c;

    for (i = 0; i < m; i++) {
        len = small ? (int)rnd(1, 3) : (int)rnd(1, 10);
        words[i] = malloc((size_t)len + 1);
        for (j = 0; j < len; j++)
            words[i][j] = rnd_char(letters);
        words[i][len] = '\0';
        s[i] = words[i][0];
    }
    n = m;
    if (shape == 1 && n > 1) {            /* one letter short */
        n--;
    } else if (shape == 2 && n < 100) {   /* one letter more */
        s[n] = rnd_char(letters);
        n++;
    } else if (shape == 3) {              /* one letter changed */
        at = small ? (int)rnd(0, n - 1) : n - 1 - (int)rnd(0, 2);
        do
            c = rnd_char(letters);
        while (c == s[at]);
        s[at] = c;
    } else if (shape == 4 && n > 1) {     /* two neighbours exchanged */
        at = (int)rnd(0, n - 2);
        c = s[at];
        s[at] = s[at + 1];
        s[at + 1] = c;
    } else if (shape == 5) {              /* the last letters */
        for (i = 0; i < m; i++)
            s[i] = words[i][strlen(words[i]) - 1];
    } else if (shape == 6) {              /* anything */
        n = small ? (int)rnd(1, 7) : (int)rnd(1, 100);
        for (i = 0; i < n; i++)
            s[i] = rnd_char(letters);
    }
    s[n] = '\0';
    put_strings(words, m);
    put_string(s);
    for (i = 0; i < m; i++)
        free(words[i]);
    free(words);
}
