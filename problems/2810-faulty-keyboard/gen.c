/* Keys typed: small cases are a few of a, b, c, d and i, so that runs of
 * i, and an i at the end, come up often; full-size ones are 100 keys with
 * a share of i's the seed picks, from none to most of them, the other
 * keys from the whole alphabet. The first key is never i. */
void gen(int small)
{
    static const char *letters = "abcdefghjklmnopqrstuvwxyz";
    char s[101];
    int n = small ? (int)rnd(1, 8) : 100;
    int share = small ? 35 : (int)rnd(0, 80);
    int k, flip;

    s[0] = small ? rnd_char("abcd") : rnd_char(letters);
    for (k = 1; k < n; k++) {
        flip = rnd_chance(share);
        if (flip)
            s[k] = 'i';
        else
            s[k] = small ? rnd_char("abcd") : rnd_char(letters);
    }
    s[n] = '\0';
    put_string(s);
}
