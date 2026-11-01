/* Small cases have up to 10 letters, full-size ones 100. The seed picks
 * the shape:
 *   0. random letters, in either case, from a few letters of the alphabet
 *      (up to 3 when small, up to 26 otherwise), with some lowercase
 *      letters from outside them mixed in, which never have a partner;
 *   1. nice blocks (one letter of a..y in both cases, at least once each)
 *      separated by a lone z, all of one length or of random lengths, so
 *      that ties between blocks come up often;
 *   2. lowercase a..m and uppercase N..Y only, which leave nothing nice,
 *      often with one "Zz" or "zZ" put in somewhere;
 *   3. random letters from aAbB (small) or from all 52 letters. */
static char letter(int index, int upper)
{
    return (char)((upper ? 'A' : 'a') + index);
}

void gen(int small)
{
    char s[101], order[26];
    int n = small ? (int)rnd(1, 10) : 100;
    int shape = (int)rnd(0, 3), i, j, k, stray, idx, up, len, fixed, top, p;

    if (shape == 0) {
        for (i = 0; i < 26; i++)
            order[i] = (char)i;
        for (i = 25; i > 0; i--) {
            j = (int)rnd(0, i);
            k = order[i];
            order[i] = order[j];
            order[j] = (char)k;
        }
        k = small ? (int)rnd(1, 3) : (int)rnd(1, 26);
        stray = small ? 15 : (int)rnd(0, 8);
        for (i = 0; i < n; i++) {
            if (k < 26 && rnd_chance(stray)) {
                idx = (int)rnd(k, 25);
                s[i] = letter(order[idx], 0);
            } else {
                idx = (int)rnd(0, k - 1);
                up = rnd_chance(50);
                s[i] = letter(order[idx], up);
            }
        }
    } else if (shape == 1) {
        top = small ? 4 : 10;
        fixed = rnd_chance(50) ? (int)rnd(2, top) : 0;
        for (i = 0; i < n;) {
            if (i > 0)
                s[i++] = 'z';
            len = fixed ? fixed : (int)rnd(2, top);
            if (len > n - i)
                len = n - i;
            idx = (int)rnd(0, 24);
            for (j = 0; j < len; j++) {
                up = rnd_chance(50);
                s[i + j] = letter(idx, up);
            }
            if (len >= 2) {
                p = (int)rnd(0, len - 2);
                up = rnd_chance(50);
                s[i + p] = letter(idx, up);
                s[i + p + 1] = letter(idx, !up);
            }
            i += len;
        }
    } else if (shape == 2) {
        for (i = 0; i < n; i++) {
            up = rnd_chance(50);
            s[i] = up ? rnd_char("NOPQRSTUVWXY") : rnd_char("abcdefghijklm");
        }
        if (n >= 2 && rnd_chance(60)) {
            p = (int)rnd(0, n - 2);
            up = rnd_chance(50);
            s[p] = letter(25, up);
            s[p + 1] = letter(25, !up);
        }
    } else {
        for (i = 0; i < n; i++)
            s[i] = rnd_char(small ? "aAbB"
                                  : "abcdefghijklmnopqrstuvwxyz"
                                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    }
    s[n] = '\0';
    put_string(s);
}
