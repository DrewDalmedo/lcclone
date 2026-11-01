/* One word. Small cases have 1 to 10 letters from a, b and c, or the seed
 * makes them a few random letters followed by the same letters reversed
 * (which cut down to nothing), sometimes with one more letter in the
 * middle. Full-size cases have 10^5 letters, in a shape the seed picks:
 * "ab" repeated, then the same reversed, whose only twin pair at any time
 * is in the middle (nothing remains); a word with no twin pair at all
 * (nothing is cut); random letters from a and b (many cuts in a row); or
 * half a word with no twin pair, followed by a block that cuts down to
 * nothing one pair at a time from its middle. */

/* A letter other than prev (any letter when prev is 0). */
static char other_than(char prev)
{
    char c;

    if (!prev)
        return (char)('a' + rnd(0, 25));
    c = (char)('a' + rnd(0, 24));
    return c >= prev ? (char)(c + 1) : c;
}

/* Writes t followed by t reversed: k letters with no twin pair, the first
 * of them other than prev, then the same k in reverse. */
static void mirrored(char *w, int k, char prev)
{
    int i;

    for (i = 0; i < k; i++) {
        w[i] = other_than(prev);
        prev = w[i];
    }
    for (i = 0; i < k; i++)
        w[k + i] = w[k - 1 - i];
}

void gen(int small)
{
    int n = 100000, i, k;
    char *w = malloc(100001);

    if (small) {
        if (gen_seed() % 3 == 0) {
            k = (int)rnd(1, 4);
            for (i = 0; i < k; i++)
                w[i] = rnd_char("abc");
            n = 2 * k;
            for (i = 0; i < k; i++)
                w[n - 1 - i] = w[i];
            if (rnd_chance(50)) {
                memmove(w + k + 1, w + k, (size_t)k);
                w[k] = rnd_char("abc");
                n++;
            }
        } else {
            n = (int)rnd(1, 10);
            for (i = 0; i < n; i++)
                w[i] = rnd_char(gen_seed() % 3 == 1 ? "ab" : "abc");
        }
    } else {
        switch ((int)(gen_seed() % 4)) {
        case 0:
            for (i = 0; i < n / 2; i++)
                w[i] = i % 2 ? 'b' : 'a';
            for (i = 0; i < n / 2; i++)
                w[n - 1 - i] = w[i];
            break;
        case 1:
            for (i = 0; i < n; i++)
                w[i] = other_than(i ? w[i - 1] : 0);
            break;
        case 2:
            for (i = 0; i < n; i++)
                w[i] = rnd_char("ab");
            break;
        default:
            for (i = 0; i < n / 2; i++)
                w[i] = other_than(i ? w[i - 1] : 0);
            mirrored(w + n / 2, n / 4, w[n / 2 - 1]);
        }
    }
    w[n] = '\0';
    put_string(w);
    free(w);
}
