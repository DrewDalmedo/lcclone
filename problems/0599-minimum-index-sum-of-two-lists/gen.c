/* Two lists of distinct strings with at least one string in common. All
 * the strings are drawn different from each other; list1 takes the first
 * n1, and list2 takes `shared` of list1's strings plus new ones, in a
 * random order. Small cases use short strings over "aAb ", so that strings
 * differing only in case or by a space, prefixes and ties come up often. */
static char *pool[2000];

static void draw(int small, int count)
{
    static const char letters[] =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    char buf[31];
    int k = 0, len, i;

    while (k < count) {
        len = (int)(small ? rnd(1, 3) : rnd(1, 30));
        for (i = 0; i < len; i++)
            buf[i] = small ? rnd_char("aAb ") : rnd_char(letters);
        buf[len] = '\0';
        for (i = 0; i < k && strcmp(pool[i], buf) != 0; i++)
            ;
        if (i == k) {
            pool[k] = malloc((size_t)len + 1);
            memcpy(pool[k], buf, (size_t)len + 1);
            k++;
        }
    }
}

void gen(int small)
{
    int n1 = (int)(small ? rnd(1, 6) : 1000);
    int n2 = (int)(small ? rnd(1, 6) : 1000);
    int most = n1 < n2 ? n1 : n2;
    int shared = (int)rnd(1, small || rnd_chance(50) ? most : 5);
    int *pick = malloc((size_t)n1 * sizeof *pick);
    int *order = malloc((size_t)n2 * sizeof *order);
    char **list2 = malloc((size_t)n2 * sizeof *list2);
    int i;

    draw(small, n1 + n2 - shared);
    for (i = 0; i < n1; i++)
        pick[i] = i;
    rnd_shuffle(pick, n1);
    for (i = 0; i < n2; i++)
        order[i] = i;
    rnd_shuffle(order, n2);
    for (i = 0; i < n2; i++)
        list2[order[i]] = i < shared ? pool[pick[i]] : pool[n1 + i - shared];
    put_strings(pool, n1);
    put_strings(list2, n2);
    for (i = 0; i < n1 + n2 - shared; i++)
        free(pool[i]);
    free(pick);
    free(order);
    free(list2);
}
