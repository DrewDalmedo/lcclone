/* Two words. Small cases are a few letters long, drawn from a, b and c
 * half the time (so that equal letters hide nothing) and from the whole
 * alphabet otherwise. Full-size ones reach the length limits: the seed
 * picks both words at 100 letters, one at 100 and the other at 1, or two
 * random lengths. */
static void word(int len, const char *letters)
{
    char s[101];
    int i;

    for (i = 0; i < len; i++)
        s[i] = rnd_char(letters);
    s[len] = '\0';
    put_string(s);
}

void gen(int small)
{
    static const char *abc = "abc";
    static const char *all = "abcdefghijklmnopqrstuvwxyz";
    const char *letters = small && rnd_chance(50) ? abc : all;
    int shape = (int)rnd(0, 3), n1, n2;

    if (small) {
        n1 = (int)rnd(1, 8);
        n2 = (int)rnd(1, 8);
    } else if (shape == 0) {
        n1 = 100;
        n2 = 100;
    } else if (shape == 1) {
        n1 = 100;
        n2 = 1;
    } else if (shape == 2) {
        n1 = 1;
        n2 = 100;
    } else {
        n1 = (int)rnd(1, 100);
        n2 = (int)rnd(1, 100);
    }
    word(n1, letters);
    word(n2, letters);
}
