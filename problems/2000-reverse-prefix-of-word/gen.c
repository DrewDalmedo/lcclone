/* Small cases: up to 8 letters from "abc", and ch from "abcd", so that ch
 * is sometimes missing and often repeated. Full size: 250 letters, in a
 * shape the seed picks: ch missing; ch only at the last index; the first
 * ch past index 127; or letters from a few ones, ch among them. */

/* A random lowercase letter other than ch. */
static char other(char ch)
{
    char c = (char)('a' + rnd(0, 24));

    return c >= ch ? (char)(c + 1) : c;
}

void gen(int small)
{
    char word[251];
    int n = small ? (int)rnd(1, 8) : 250, shape, first, i;
    char ch, few[6];

    if (small) {
        for (i = 0; i < n; i++)
            word[i] = rnd_char("abc");
        ch = rnd_char("abcd");
    } else {
        shape = (int)rnd(0, 3);
        ch = (char)('a' + rnd(0, 25));
        first = shape == 1 ? n - 1 : shape == 2 ? (int)rnd(128, n - 1) : n;
        for (i = 0; i < 5; i++)
            few[i] = i == 0 ? ch : other(ch);
        few[5] = '\0';
        for (i = 0; i < n; i++) {
            if (shape == 3)
                word[i] = rnd_char(few);
            else if (i < first)
                word[i] = other(ch);
            else if (i == first)
                word[i] = ch;
            else
                word[i] = (char)('a' + rnd(0, 25));
        }
    }
    word[n] = '\0';
    put_string(word);
    put_char(ch);
}
