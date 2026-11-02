/* A string drawn from a pool of letters the seed picks: vowels only,
 * consonants only (y included), a few of each, or the whole alphabet.
 * Small cases use short strings over few letters, so that counts tie
 * often; full-size ones are 100 letters long. */
void gen(int small)
{
    static const char *tiny[] = {"aeiou", "bcdy", "aeby", "uoyz", "ayb"};
    static const char *wide[] = {
        "aeiou", "bcdfghjklmnpqrstvwxyz", "abcdefghijklmnopqrstuvwxyz",
        "aaeeiioouuy", "eyyz",
    };
    const char *pool;
    char s[101];
    int n, i;

    if (small) {
        n = (int)rnd(1, 8);
        pool = tiny[rnd(0, 4)];
    } else {
        n = 100;
        pool = wide[rnd(0, 4)];
    }
    for (i = 0; i < n; i++)
        s[i] = rnd_char(pool);
    s[n] = '\0';
    put_string(s);
}
