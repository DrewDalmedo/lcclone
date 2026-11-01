/* A word of vowels, consonants (y and Y often), digits and, in some cases,
 * symbols. Small cases are 1 to 6 characters long, and each case weighs
 * the four kinds differently, so that words missing a vowel, a consonant,
 * or holding a symbol come up often. Full-size cases are 20 characters
 * long, and the seed picks a shape where the deciding character is the
 * last one: the only vowel, the only consonant or a symbol there. */
static char pick(int kind)
{
    switch (kind) {
    case 0:
        return rnd_char("aeiouAEIOU");
    case 1:
        if (rnd_chance(25))
            return rnd_char("yY");
        return rnd_char("bcdfghjklmnpqrstvwxzBCDFGHJKLMNPQRSTVWXZ");
    case 2:
        return rnd_char("0123456789");
    default:
        return rnd_char("@#$");
    }
}

void gen(int small)
{
    char word[21];
    int w[4], total, n, i, r, kind, shape;

    w[0] = (int)rnd(0, 3);
    w[1] = (int)rnd(0, 3);
    w[2] = (int)rnd(0, 3);
    w[3] = rnd_chance(30) ? 1 : 0;
    total = w[0] + w[1] + w[2] + w[3];
    if (total == 0) {
        w[2] = 1;
        total = 1;
    }
    n = small ? (int)rnd(1, 6) : 20;
    for (i = 0; i < n; i++) {
        r = (int)rnd(0, total - 1);
        for (kind = 0; r >= w[kind]; kind++)
            r -= w[kind];
        word[i] = pick(kind);
    }
    if (!small) {
        /* 0: as drawn; 1: the only vowel last; 2: the only consonant
         * last; 3: letters and digits, then a symbol last. */
        shape = (int)rnd(0, 3);
        for (i = 0; shape != 0 && i < n - 1; i++) {
            kind = (int)rnd(0, 1);
            if (shape == 1)
                word[i] = pick(kind == 0 ? 1 : 2);
            else if (shape == 2)
                word[i] = pick(kind == 0 ? 0 : 2);
            else
                word[i] = pick(i % 3);
        }
        if (shape != 0)
            word[n - 1] = pick(shape == 1 ? 0 : shape == 2 ? 1 : 3);
    }
    word[n] = '\0';
    put_string(word);
}
