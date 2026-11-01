/* A trip through n + 1 different cities, printed as its n legs in a
 * shuffled order. Small cases: 1 to 6 legs, names of 1 to 3 characters
 * from a, A and b (a space can sit between two of them), so that names
 * that are prefixes or case variants of each other come up often.
 * Full-size cases: 100 legs; the seed picks the names (any letters and
 * inner spaces, 1 to 10 characters; 10 characters sharing the prefix
 * "Port"; or 1 to 10 characters of a and A) and whether the leg into the
 * last city is listed anywhere, last or first. */

/* Writes a random name of minlen to maxlen characters, drawn from
 * letters, to s. With spaces set, a character that is neither the first
 * nor the last may be a space instead. */
static void make_name(char *s, int minlen, int maxlen, const char *letters,
                      int spaces)
{
    int len = (int)rnd(minlen, maxlen), k;

    for (k = 0; k < len; k++) {
        if (spaces && k > 0 && k < len - 1 && rnd_chance(15))
            s[k] = ' ';
        else
            s[k] = rnd_char(letters);
    }
    s[len] = '\0';
}

void gen(int small)
{
    static const char *any =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int n = small ? (int)rnd(1, 6) : 100, shape = (int)rnd(0, 2);
    int where = (int)rnd(0, 2), order[100], i, k, dup, tmp;
    char names[101][11];

    for (i = 0; i <= n; i++) {
        do {
            if (small) {
                make_name(names[i], 1, 3, "aAb", 1);
            } else if (shape == 0) {
                make_name(names[i], 1, 10, any, 1);
            } else if (shape == 1) {
                memcpy(names[i], "Port", 4);
                make_name(names[i] + 4, 6, 6, "aAbB", 0);
            } else {
                make_name(names[i], 1, 10, "aA", 0);
            }
            dup = 0;
            for (k = 0; k < i; k++)
                if (strcmp(names[k], names[i]) == 0)
                    dup = 1;
        } while (dup);
    }
    /* Leg k goes from names[k] to names[k + 1]. */
    for (k = 0; k < n; k++)
        order[k] = k;
    rnd_shuffle(order, n);
    if (!small && where > 0) {
        for (k = 0; order[k] != n - 1; k++)
            ;
        i = where == 1 ? n - 1 : 0;
        tmp = order[i];
        order[i] = order[k];
        order[k] = tmp;
    }
    putchar('[');
    for (k = 0; k < n; k++)
        printf("%s[\"%s\",\"%s\"]", k ? "," : "", names[order[k]],
               names[order[k] + 1]);
    puts("]");
}
