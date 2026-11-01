/* Picks d distinct letters and a common count c, so that about half the
 * cases are true; the rest are spoiled by giving one letter one vote more
 * or fewer, by a difference of 256 (it wraps a byte-sized counter), or by
 * random counts. Small cases use a few letters of a-f with counts up to 3;
 * full-size ones up to all 26 letters and lengths near 1000. Sometimes the
 * odd letter's votes are all put at the very end. */
void gen(int small)
{
    int idx[26], count[26], order[1000];
    char s[1001];
    int d, c, n = 0, shape, odd, tail = 0, i, k;

    for (i = 0; i < 26; i++)
        idx[i] = i;
    rnd_shuffle(idx, small ? 6 : 26);
    d = small ? (int)rnd(1, 4) : (int)rnd(1, 26);
    c = small ? (int)rnd(1, 3) : (rnd_chance(50) ? 1000 / d : (int)rnd(1, 1000 / d));
    for (i = 0; i < d; i++)
        count[i] = c;
    shape = (int)rnd(0, 4);
    odd = (int)rnd(0, d - 1);
    if (shape == 1) {
        if (d * c < 1000)
            count[odd]++;
        else
            count[odd]--;
    } else if (shape == 2) {
        if (c > 1)
            count[odd]--;
        else if (d * c < 1000)
            count[odd]++;
    } else if (shape == 3) {
        if (!small && d * c + 256 <= 1000)
            count[odd] += 256;
        else if (!small && c > 256)
            count[odd] -= 256;
        else
            count[odd] = (int)rnd(1, small ? 4 : 1000 / d);
    } else if (shape == 4) {
        for (i = 0; i < d; i++)
            count[i] = (int)rnd(1, small ? 3 : 1000 / d);
    }
    tail = rnd_chance(30) ? count[odd] : 0;
    for (i = 0; i < d; i++)
        for (k = 0; k < count[i] - (i == odd ? tail : 0); k++)
            order[n++] = idx[i];
    rnd_shuffle(order, n);
    for (k = 0; k < tail; k++)
        order[n++] = idx[odd];
    for (i = 0; i < n; i++)
        s[i] = (char)('a' + order[i]);
    s[n] = '\0';
    put_string(s);
}
