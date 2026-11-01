/* Small cases: up to 6 letters, weighted towards j-z (two-digit ranks,
 * those with a 0 digit, and s, whose rank 19 has the largest digit sum),
 * and k mostly 1 to 3 so the result is often still several digits.
 * Full-size cases: 100 letters, either random or mostly s, with k
 * anywhere from 1 to 10 but often 1 or 2. */
void gen(int small)
{
    char s[101];
    int n = small ? (int)rnd(1, 6) : 100, heavy = rnd_chance(40), k, i;

    for (i = 0; i < n; i++) {
        if (heavy && rnd_chance(80))
            s[i] = 's';
        else if (rnd_chance(50))
            s[i] = rnd_char("jstz");
        else
            s[i] = rnd_char("abcdefghijklmnopqrstuvwxyz");
    }
    s[n] = '\0';
    k = rnd_chance(60) ? (int)rnd(1, small ? 3 : 2) : (int)rnd(1, 10);
    put_string(s);
    put_int(k);
}
