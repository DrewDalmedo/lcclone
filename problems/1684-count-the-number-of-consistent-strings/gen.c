/* allowed is a few distinct letters taken from a shuffled alphabet. Small
 * cases draw from the five letters a, b, c, y, z, with short words that
 * mostly use allowed's letters, so words repeat and both answers come up.
 * Full-size cases have 10^4 words; the seed picks allowed of all 26
 * letters, of 20 to 25, or of any length, and each word of 10 letters
 * either fits allowed or breaks it only with its last letter, so a
 * letter-by-letter check has to read every word to the end. */
void gen(int small)
{
    static char store[10000][11];
    char *words[10000], allowed[27], outside[27];
    const char *pool = small ? "abcyz" : "abcdefghijklmnopqrstuvwxyz";
    int letters[26], np = (int)strlen(pool), m, nout = 0, n, len, shape;
    int i, j, pick;

    for (i = 0; i < np; i++)
        letters[i] = i;
    rnd_shuffle(letters, np);
    if (small) {
        m = (int)rnd(1, np - 1);
    } else {
        shape = (int)rnd(0, 2);
        m = shape == 0 ? 26 : shape == 1 ? (int)rnd(20, 25) : (int)rnd(1, 26);
    }
    for (i = 0; i < np; i++) {
        if (i < m)
            allowed[i] = pool[letters[i]];
        else
            outside[nout++] = pool[letters[i]];
    }
    allowed[m] = '\0';
    n = small ? (int)rnd(1, 8) : 10000;
    for (i = 0; i < n; i++) {
        len = small ? (int)rnd(1, 4) : 10;
        for (j = 0; j < len; j++) {
            if (small && rnd_chance(20)) {
                pick = (int)rnd(0, np - 1);
                store[i][j] = pool[pick];
            } else {
                pick = (int)rnd(0, m - 1);
                store[i][j] = allowed[pick];
            }
        }
        if (!small && nout > 0 && rnd_chance(50)) {
            pick = (int)rnd(0, nout - 1);
            store[i][len - 1] = outside[pick];
        }
        store[i][len] = '\0';
        words[i] = store[i];
    }
    put_string(allowed);
    put_strings(words, n);
}
