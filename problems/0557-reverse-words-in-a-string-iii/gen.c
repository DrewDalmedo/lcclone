/* A line of words joined by single spaces. Small cases have 1 to 4 words
 * of 1 to 4 characters, from a few letters of both cases, a digit, a quote,
 * a backslash and a dot. Full-size ones fill 5 * 10^4 characters, and the
 * seed picks the shape: 25000 one-character words, a single word, or words
 * of random lengths, all from printable ASCII. */
void gen(int small)
{
    int limit = small ? 20 : 50000, len = 0, words, w, wl, i, shape;
    char *s = malloc((size_t)limit + 1);

    shape = small ? 2 : (int)(gen_seed() % 3);
    words = small ? (int)rnd(1, 4) : limit;
    for (w = 0; w < words; w++) {
        if (shape == 0)
            wl = 1;
        else if (shape == 1)
            wl = limit;
        else
            wl = small ? (int)rnd(1, 4) : (int)rnd(1, 12);
        if (w > 0) {
            if (len + 1 >= limit)
                break;
            s[len++] = ' ';
        }
        if (wl > limit - len)
            wl = limit - len;
        for (i = 0; i < wl; i++)
            s[len++] = small ? rnd_char("abXY7\"\\.") : (char)rnd(33, 126);
    }
    s[len] = '\0';
    put_string(s);
    free(s);
}
