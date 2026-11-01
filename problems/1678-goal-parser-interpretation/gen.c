/* Tokens one after another. Small cases have 1 to 8 tokens, each of the
 * three forms as likely. Full-size cases fill exactly 100 characters, in a
 * mix the seed picks: the three forms as likely; one form throughout;
 * "()" and "(al)" in a random order, with a 'G' at the end only when
 * nothing else fits; or mostly 'G' with a few others. */
static const char *tokens[] = {"G", "()", "(al)"};

void gen(int small)
{
    char s[101];
    int len = 0, count, shape, only, t, k;

    if (small) {
        count = (int)rnd(1, 8);
        for (k = 0; k < count; k++) {
            t = (int)rnd(0, 2);
            strcpy(s + len, tokens[t]);
            len += (int)strlen(tokens[t]);
        }
    } else {
        shape = (int)(gen_seed() % 4);
        only = (int)rnd(0, 2);
        while (len < 100) {
            if (shape == 0)
                t = (int)rnd(0, 2);
            else if (shape == 1)
                t = only;
            else if (shape == 2)
                t = (int)rnd(1, 2);
            else
                t = rnd_chance(80) ? 0 : (int)rnd(1, 2);
            if (len + (int)strlen(tokens[t]) > 100)
                t = len + 2 <= 100 && shape != 1 ? 1 : 0;
            strcpy(s + len, tokens[t]);
            len += (int)strlen(tokens[t]);
        }
    }
    put_string(s);
}
