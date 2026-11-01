/* Letters mostly from A to D, so pairs cancel, nest and cross, with now
 * and then a blocking letter X. Some strings are built as properly nested
 * AB and CD pairs, which cancel completely, and then may get a few
 * letters changed. Small cases have up to 10 letters. Full-size ones have
 * 100, and the seed picks: random A to D, fully nested (the answer is 0,
 * and nests deep enough to need many rounds of find-and-delete), nested
 * with a few changes, no removable pair at all (the answer is 100), or
 * random A to Z. */

/* Fills s[0..len) with nested pairs; len is even. */
static void nested(char *s, int len)
{
    char stack[100];
    int top = 0, i, close;
    char c;

    for (i = 0; i < len; i++) {
        if (top == 0)
            close = 0;
        else if (top == len - i)
            close = 1;
        else
            close = rnd_chance(50);
        if (close) {
            top--;
            s[i] = stack[top] == 'A' ? 'B' : 'D';
        } else {
            c = rnd_char("AC");
            stack[top++] = c;
            s[i] = c;
        }
    }
}

/* Changes a few letters of s[0..len) to random ones. */
static void mutate(char *s, int len, int times)
{
    int i, at;

    for (i = 0; i < times; i++) {
        at = (int)rnd(0, len - 1);
        s[at] = rnd_char("ABCDX");
    }
}

void gen(int small)
{
    char s[101];
    int shape = (int)rnd(0, 4), len, i;
    char c;

    if (small) {
        len = (int)rnd(1, 10);
        if (shape == 0) {
            for (i = 0; i < len; i++)
                s[i] = rnd_char("ABCD");
        } else if (shape == 1) {
            for (i = 0; i < len; i++)
                s[i] = rnd_char("ABCDX");
        } else {
            len = 2 * (int)rnd(1, 5);
            nested(s, len);
            if (shape == 3)
                mutate(s, len, 1);
            else if (shape == 4)
                mutate(s, len, 2);
        }
    } else {
        len = 100;
        if (shape == 0) {
            for (i = 0; i < len; i++)
                s[i] = rnd_char("ABCD");
        } else if (shape == 1) {
            nested(s, len);
        } else if (shape == 2) {
            nested(s, len);
            mutate(s, len, (int)rnd(1, 4));
        } else if (shape == 3) {
            for (i = 0; i < len; i++) {
                do
                    c = rnd_char("ABCDX");
                while (i > 0 && ((s[i - 1] == 'A' && c == 'B') ||
                                 (s[i - 1] == 'C' && c == 'D')));
                s[i] = c;
            }
        } else {
            for (i = 0; i < len; i++)
                s[i] = rnd_char("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        }
    }
    s[len] = '\0';
    put_string(s);
}
