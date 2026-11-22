/* Brackets that are mostly valid, so that the answer is often true. Small
 * cases are up to ten brackets: a valid string, sometimes with one
 * bracket changed, or random brackets. Full-size ones are 10^4 brackets,
 * in a shape the seed picks: 5000 open brackets and then their closing
 * ones (as deep as it gets); the same with the last bracket of the wrong
 * kind; a random valid string; or a random valid string with one more
 * open bracket at the start and a closing one of the wrong kind at the
 * end, so that the mistake shows only at the very end. */
static const char opens[] = "([{", closes[] = ")]}";

/* Fills s[0..n) (n even) with a random valid string; deep makes it all
 * open brackets first, then the closing ones. */
static void valid(char *s, int n, int deep)
{
    char *stack = malloc((size_t)n / 2 + 1);
    int i, top = 0, k;

    for (i = 0; i < n; i++) {
        int open = deep ? i < n / 2
                 : top == 0 ? 1
                 : top == n - i ? 0
                 : rnd_chance(50);

        if (open) {
            k = (int)rnd(0, 2);
            stack[top++] = closes[k];
            s[i] = opens[k];
        } else {
            s[i] = stack[--top];
        }
    }
    free(stack);
}

/* Any bracket but c. */
static char other(char c)
{
    char d;

    do
        d = rnd_char("()[]{}");
    while (d == c);
    return d;
}

void gen(int small)
{
    int n, i;
    char *s = malloc(10001);

    if (small) {
        n = (int)rnd(1, 10);
        switch (rnd(0, 2)) {
        case 0:
            for (i = 0; i < n; i++)
                s[i] = rnd_char("()[]{}");
            break;
        default:
            n -= n % 2;
            if (n == 0)
                n = 2;
            valid(s, n, 0);
            if (rnd_chance(50)) {
                i = (int)rnd(0, n - 1);
                s[i] = other(s[i]);
            }
            break;
        }
    } else {
        n = 10000;
        switch (gen_seed() % 4) {
        case 0:
            valid(s, n, 1);
            break;
        case 1:
            valid(s, n, 1);
            s[n - 1] = s[n - 1] == ')' ? rnd_char("]}")
                     : s[n - 1] == ']' ? rnd_char(")}") : rnd_char(")]");
            break;
        case 2:
            valid(s, n, 0);
            break;
        default:
            valid(s + 1, n - 2, 0);
            i = (int)rnd(0, 2);
            s[0] = opens[i];
            s[n - 1] = closes[(i + (int)rnd(1, 2)) % 3];
            break;
        }
    }
    s[n] = '\0';
    put_string(s);
    free(s);
}
