/* n strings, 1 to 5 of them for small cases and 100 (or, now and then,
 * fewer) for full-size ones, using a few letters picked at random from
 * the whole alphabet: 1 to 3 for small cases, 1 to 26 otherwise. The seed
 * picks the shape:
 *   - random strings of 1 to 4 letters (small) or 1 to 100;
 *   - n copies of one random word of k letters (k up to 3 for small
 *     cases, up to 100 and often exactly 100 otherwise), with all their
 *     letters shuffled together and handed out again in strings of random
 *     lengths from 1 to 100, so the answer is true;
 *   - the same, then one letter changed to another, which leaves the
 *     total length a multiple of n but spoils two letters' counts. */
void gen(int small)
{
    static char buf[100][101];
    static int pool[10000];
    char *words[100];
    int perm[26], base[100], len[100];
    int n, letters, shape, k, total, moves, a, b, pos, shift, idx, i, j;

    n = small ? (int)rnd(1, 5) : (rnd_chance(80) ? 100 : (int)rnd(1, 99));
    letters = small ? (int)rnd(1, 3) : (int)rnd(1, 26);
    for (i = 0; i < 26; i++)
        perm[i] = i;
    rnd_shuffle(perm, 26);
    shape = (int)rnd(0, 2);
    if (shape == 0) {
        total = 0;
        for (i = 0; i < n; i++) {
            len[i] = (int)rnd(1, small ? 4 : 100);
            for (j = 0; j < len[i]; j++)
                pool[total++] = perm[rnd(0, letters - 1)];
        }
    } else {
        k = small ? (int)rnd(1, 3) : (rnd_chance(50) ? 100 : (int)rnd(1, 100));
        for (j = 0; j < k; j++)
            base[j] = perm[rnd(0, letters - 1)];
        total = n * k;
        for (i = 0; i < total; i++)
            pool[i] = base[i % k];
        rnd_shuffle(pool, total);
        for (i = 0; i < n; i++)
            len[i] = k;
        moves = (int)rnd(0, small ? 6 : 3 * n);
        for (i = 0; i < moves; i++) {
            a = (int)rnd(0, n - 1);
            b = (int)rnd(0, n - 1);
            if (a != b && len[a] > 1 && len[b] < 100) {
                len[a]--;
                len[b]++;
            }
        }
        if (shape == 2) {
            pos = (int)rnd(0, total - 1);
            shift = (int)rnd(1, 25);
            pool[pos] = (pool[pos] + shift) % 26;
        }
    }
    idx = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < len[i]; j++)
            buf[i][j] = (char)('a' + pool[idx++]);
        buf[i][len[i]] = '\0';
        words[i] = buf[i];
    }
    put_strings(words, n);
}
