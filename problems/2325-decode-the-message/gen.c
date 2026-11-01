/* A key that brings in the 26 letters in a random order, with spaces and
 * repeats of letters already met mixed in, and a message of letters and
 * spaces. The seed picks the key's shape: repeats and spaces spread
 * through it; a long stretch of one to three letters (and spaces) before
 * the rest of the alphabet shows up at the very end; or exactly 26
 * letters. Small cases have keys of 26 to 40 characters and messages of 1
 * to 12, sometimes all spaces; full-size ones a message of 2000 and,
 * unless the key is 26 letters, a key of 2000. */
void gen(int small)
{
    static char key[2001], msg[2001];
    int perm[26], len, m, shape, space_pct, early, placed = 0, i, r, left;

    for (i = 0; i < 26; i++)
        perm[i] = i;
    rnd_shuffle(perm, 26);
    shape = small ? (int)rnd(0, 3) % 3 : (int)rnd(0, 2);
    len = shape == 2 ? 26 : small ? (int)rnd(27, 40) : 2000;
    space_pct = (int)rnd(10, 60);
    if (shape == 1) {
        early = (int)rnd(1, 3);
        for (i = 0; i < early; i++)
            key[i] = (char)('a' + perm[i]);
        for (; i < len - 26 + early; i++) {
            r = (int)rnd(0, early - 1);
            key[i] = rnd_chance(20) ? ' ' : (char)('a' + perm[r]);
        }
        for (placed = early; placed < 26; placed++)
            key[i++] = (char)('a' + perm[placed]);
    } else {
        for (i = 0; i < len; i++) {
            left = 26 - placed;
            r = (int)rnd(1, len - i);
            if (r <= left) {
                key[i] = (char)('a' + perm[placed++]);
            } else if (placed == 0 || rnd_chance(space_pct)) {
                key[i] = ' ';
            } else {
                r = (int)rnd(0, placed - 1);
                key[i] = (char)('a' + perm[r]);
            }
        }
    }
    key[len] = '\0';
    m = small ? (int)rnd(1, 12) : 2000;
    space_pct = small && rnd_chance(5) ? 100 : (int)rnd(5, 35);
    for (i = 0; i < m; i++)
        msg[i] = rnd_chance(space_pct) ? ' ' : (char)('a' + rnd(0, 25));
    msg[m] = '\0';
    put_string(key);
    put_string(msg);
}
