/* Sorted letters with at least two different ones, and a target. Small
 * cases are up to eight letters from "a" to "f", with a target from "a" to
 * "g". Full-size ones are 10^4 letters, in a shape the seed picks: all "a"
 * but the last few, which are "z", with "y" as the target, so the answer
 * is at the far end; a target at or after the last letter, so the answer
 * wraps around to the first; random letters of the whole alphabet and a
 * random target; or "a" repeated and a single "b" at the end, with "a" as
 * the target. */
static void sorted(char *s, int n, char lo, char hi)
{
    int count[26] = {0}, i, k = 0, c;

    do {
        for (i = 0; i < n; i++)
            s[i] = (char)rnd(lo, hi);
        for (c = 0; c < 26; c++)
            count[c] = 0;
        for (i = 0; i < n; i++)
            count[s[i] - 'a']++;
        for (c = 0, k = 0; c < 26; c++)
            k += count[c] > 0;
    } while (k < 2);
    for (c = 0, k = 0; c < 26; c++)
        for (i = 0; i < count[c]; i++)
            s[k++] = (char)('a' + c);
}

void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 10000, i, tail;
    char *s = malloc((size_t)n), target;

    if (small) {
        sorted(s, n, 'a', 'f');
        target = (char)rnd('a', 'g');
    } else {
        switch (gen_seed() % 4) {
        case 0:
            tail = (int)rnd(1, 5);
            for (i = 0; i < n; i++)
                s[i] = i < n - tail ? 'a' : 'z';
            target = 'y';
            break;
        case 1:
            sorted(s, n, 'a', (char)rnd('b', 'z'));
            target = (char)rnd(s[n - 1], 'z');
            break;
        case 2:
            sorted(s, n, 'a', 'z');
            target = (char)rnd('a', 'z');
            break;
        default:
            for (i = 0; i < n; i++)
                s[i] = 'a';
            s[n - 1] = 'b';
            target = 'a';
            break;
        }
    }
    put_chars(s, n);
    put_char(target);
    free(s);
}
