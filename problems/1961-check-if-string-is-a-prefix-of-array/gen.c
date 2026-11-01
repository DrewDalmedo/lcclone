/* Words of a and b (and c at full size), and an s made from the joined
 * words: exactly the first k of them, or that text one letter short, one
 * letter longer, or with one letter changed; sometimes a random string.
 * Small cases have up to 5 words of 1 to 3 letters. Full-size cases have
 * 100 words of up to 20 letters (often exactly 20, so the joined words
 * reach 2000 letters), and k as large as s's 1000 letters allow. */
void gen(int small)
{
    const char *letters = small ? "ab" : "abc";
    int m = small ? (int)rnd(1, 5) : 100;
    int full = !small && rnd_chance(50);
    int shape = (int)rnd(0, 4), total = 0, k, n, len, i, j, at;
    char **words = malloc((size_t)m * sizeof *words);
    char *text = malloc(2001), *s = malloc(1002), c;

    for (i = 0; i < m; i++) {
        len = small ? (int)rnd(1, 3) : full ? 20 : (int)rnd(1, 20);
        words[i] = malloc((size_t)len + 1);
        for (j = 0; j < len; j++)
            words[i][j] = rnd_char(letters);
        words[i][len] = '\0';
        memcpy(text + total, words[i], (size_t)len);
        total += len;
    }
    /* n: the length of the first k words joined. */
    if (small) {
        k = (int)rnd(1, m);
        for (n = 0, i = 0; i < k; i++)
            n += (int)strlen(words[i]);
    } else {
        for (n = 0, i = 0; i < m && n + (int)strlen(words[i]) <= 1000; i++)
            n += (int)strlen(words[i]);
    }
    memcpy(s, text, (size_t)n);
    if (shape == 1 && n > 1) {          /* one letter short */
        n--;
    } else if (shape == 2 && n < 1000) { /* one letter more */
        s[n] = n < total ? text[n] : rnd_char(letters);
        n++;
    } else if (shape == 3) {             /* one letter changed */
        at = (int)rnd(0, n - 1);
        do
            c = rnd_char(letters);
        while (c == s[at]);
        s[at] = c;
    } else if (shape == 4) {             /* anything */
        n = small ? (int)rnd(1, 8) : (int)rnd(1, 1000);
        for (i = 0; i < n; i++)
            s[i] = rnd_char(letters);
    }
    s[n] = '\0';
    put_string(s);
    put_strings(words, m);
    for (i = 0; i < m; i++)
        free(words[i]);
    free(words);
    free(text);
    free(s);
}
