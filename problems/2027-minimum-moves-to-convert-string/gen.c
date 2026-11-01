/* Small cases are 3 to 14 characters, with a share of 'X' the seed picks
 * (none to all), so that every pattern of a few characters comes up.
 * Full-size cases have 1000 characters, in a shape the seed picks: random
 * with a random share of 'X'; all 'X'; an 'X' at every third place from
 * the first (both need 334 stamps); 'X's spread out with some always in
 * the last two places; or all 'O' but the very last character. */
void gen(int small)
{
    int n = small ? (int)rnd(3, 14) : 1000, share, shape, i;
    char *s = malloc((size_t)n + 1);

    shape = small ? 0 : (int)(gen_seed() % 5);
    share = (int)rnd(0, 100);
    for (i = 0; i < n; i++) {
        if (shape == 0)
            s[i] = rnd_chance(share) ? 'X' : 'O';
        else if (shape == 1)
            s[i] = 'X';
        else if (shape == 2)
            s[i] = i % 3 == 0 ? 'X' : 'O';
        else if (shape == 3)
            s[i] = i >= n - 2 || rnd_chance(20) ? 'X' : 'O';
        else
            s[i] = i == n - 1 ? 'X' : 'O';
    }
    s[n] = '\0';
    put_string(s);
    free(s);
}
