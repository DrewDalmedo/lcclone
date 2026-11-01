/* Strings of the allowed characters and spaces. Small cases are 0 to 12
 * characters from "a1-'," and many spaces, so that empty strings, all
 * spaces, and leading, doubled and trailing spaces come up often.
 * Full-size ones are 300 characters, in a shape the seed picks: only
 * spaces, no space at all, one-character segments between single spaces
 * (150 of them), random segments and gaps, or random characters. */
static const char chars[] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
    "!@#$%^&*()_+-=',.:";

void gen(int small)
{
    int n = small ? (int)rnd(0, 12) : 300, i, k, len, first;
    char *s = malloc((size_t)n + 1);

    if (small) {
        for (i = 0; i < n; i++)
            s[i] = rnd_chance(45) ? ' ' : rnd_char("a1-',");
    } else {
        switch (gen_seed() % 5) {
        case 0: /* nothing but spaces */
            for (i = 0; i < n; i++)
                s[i] = ' ';
            break;
        case 1: /* one segment of 300 characters */
            for (i = 0; i < n; i++)
                s[i] = rnd_char(chars);
            break;
        case 2: /* 150 one-character segments, a space before or after each */
            first = (int)rnd(0, 1);
            for (i = 0; i < n; i++)
                s[i] = i % 2 == first ? rnd_char(chars) : ' ';
            break;
        case 3: /* random segments and gaps, maybe spaces at both ends */
            i = (int)rnd(0, 6);
            for (k = 0; k < i; k++)
                s[k] = ' ';
            while (i < n) {
                len = (int)rnd(1, 10);
                for (k = 0; k < len && i < n; k++)
                    s[i++] = rnd_char(chars);
                len = (int)rnd(1, 4);
                for (k = 0; k < len && i < n; k++)
                    s[i++] = ' ';
            }
            break;
        default: /* random characters, about a third of them spaces */
            for (i = 0; i < n; i++)
                s[i] = rnd_chance(33) ? ' ' : rnd_char(chars);
            break;
        }
    }
    s[n] = '\0';
    put_string(s);
    free(s);
}
