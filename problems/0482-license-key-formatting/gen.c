/* A key and a block size k. Small cases are up to 12 characters from
 * "azAZ09-", with a share of dashes from none to all, and k from 1 to 5
 * (sometimes 10^4). Full-size keys have 10^5 characters, in a shape the
 * seed picks: no dash at all with k = 1, the longest answer (199999
 * characters, slow for a solution that builds it by inserting at the
 * front); random letters, digits and dashes; blocks of one size already
 * joined by dashes, with another k; or nearly all dashes. */
static const char keychars[] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

void gen(int small)
{
    int n = small ? (int)rnd(1, 12) : 100000, k, dash, block, i;
    char *s = malloc((size_t)n + 1);

    if (small) {
        dash = (int)rnd(0, 4) * 25;
        for (i = 0; i < n; i++)
            s[i] = rnd_chance(dash) ? '-' : rnd_char("azAZ09");
        k = rnd_chance(10) ? 10000 : (int)rnd(1, 5);
    } else {
        switch (gen_seed() % 4) {
        case 0: /* no dash, k = 1 */
            for (i = 0; i < n; i++)
                s[i] = rnd_char(keychars);
            k = 1;
            break;
        case 1: /* random characters and dashes */
            dash = (int)rnd(1, 60);
            for (i = 0; i < n; i++)
                s[i] = rnd_chance(dash) ? '-' : rnd_char(keychars);
            k = (int)rnd(1, 10000);
            break;
        case 2: /* already in blocks of one size, reformatted to another */
            block = (int)rnd(1, 10);
            for (i = 0; i < n; i++)
                s[i] = (i + 1) % (block + 1) == 0 ? '-' : rnd_char(keychars);
            k = rnd_chance(50) ? (int)rnd(1, 10) : (int)rnd(1, 10000);
            break;
        default: /* dashes, with at most a few characters among them */
            for (i = 0; i < n; i++)
                s[i] = '-';
            block = (int)rnd(0, 5);
            for (i = 0; i < block; i++) {
                int at = (int)rnd(0, n - 1);

                s[at] = rnd_char(keychars);
            }
            k = (int)rnd(1, 10000);
            break;
        }
    }
    s[n] = '\0';
    put_string(s);
    put_int(k);
    free(s);
}
