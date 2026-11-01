/* Small cases: up to 6 short words over a-e, and up to 3 broken letters
 * from a-f (now and then all 26), so words often share letters and broken
 * letters sit at the start or end of words. Full-size cases pick a shape:
 * text filled to 10^4 characters with words of random length, 5000
 * one-letter words, a single word of 10^4 letters, or a random length; the
 * broken letters are any number of distinct letters. */
static char text[10001];

static void put_broken(int k, int from)
{
    int idx[26], i;
    char broken[27];

    for (i = 0; i < from; i++)
        idx[i] = i;
    rnd_shuffle(idx, from);
    for (i = 0; i < k; i++)
        broken[i] = (char)('a' + idx[i]);
    broken[k] = '\0';
    put_string(broken);
}

void gen(int small)
{
    int len = 0, words, wl, target, maxlen, shape, i, k;

    if (small) {
        words = (int)rnd(1, 6);
        for (i = 0; i < words; i++) {
            if (i > 0)
                text[len++] = ' ';
            wl = (int)rnd(1, 4);
            for (k = 0; k < wl; k++)
                text[len++] = rnd_char("abcde");
        }
        text[len] = '\0';
        put_string(text);
        if (rnd_chance(8))
            put_broken(26, 26);
        else
            put_broken((int)rnd(0, 3), 6);
        return;
    }
    shape = (int)rnd(0, 3);
    if (shape == 1) {
        for (i = 0; i < 5000; i++) {
            if (i > 0)
                text[len++] = ' ';
            text[len++] = rnd_char("abcdefghijklmnopqrstuvwxyz");
        }
    } else if (shape == 2) {
        for (i = 0; i < 10000; i++)
            text[len++] = rnd_char("abcdefghijklmnopqrstuvwxyz");
    } else {
        target = shape == 0 ? 10000 : (int)rnd(1, 10000);
        maxlen = (int)rnd(1, 12);
        for (;;) {
            wl = (int)rnd(1, maxlen);
            if (len == 0 && wl > target)
                wl = target;
            if (len > 0 && len + 1 + wl > target)
                break;
            if (len > 0)
                text[len++] = ' ';
            for (k = 0; k < wl; k++)
                text[len++] = rnd_char("abcdefghijklmnopqrstuvwxyz");
        }
    }
    text[len] = '\0';
    put_string(text);
    put_broken((int)rnd(0, 26), 26);
}
