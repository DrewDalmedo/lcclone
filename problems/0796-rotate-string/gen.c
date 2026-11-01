/* s is random, and periodic (a block written out again and again) three
 * times in ten. goal is made from it in a way the seed picks:
 *   0-2. a rotation of s, from a random starting letter;
 *   3.   a rotation with one letter redrawn (the last letter half the
 *        time), which nearly always makes it no rotation;
 *   4.   s reversed, or 5. s shuffled: the same letters, other orders;
 *   6.   a random string of the length of s;
 *   7.   a piece of s + s one letter longer or shorter than s, so that
 *        only the lengths tell it apart from a rotation.
 * Small cases use 1 to 6 letters from "ab" or "abc"; full-size ones 100
 * letters (99 and 100 when the lengths differ) from an alphabet of the
 * first 1 to 26 letters. */
static char s[101], goal[102];

static void swap(int i, int j)
{
    char t = goal[i];

    goal[i] = goal[j];
    goal[j] = t;
}

void gen(int small)
{
    static const char *const small_alphabets[] = {"ab", "abc"};
    char alphabet[27];
    int shape = (int)rnd(0, 7), n, len, letters, period, k, at, i, j;
    char c;

    if (small) {
        n = (int)rnd(1, 6);
        strcpy(alphabet, small_alphabets[rnd(0, 1)]);
    } else {
        n = shape == 7 && rnd_chance(50) ? 99 : 100;
        letters = (int)rnd(1, 26);
        for (i = 0; i < letters; i++)
            alphabet[i] = (char)('a' + i);
        alphabet[letters] = '\0';
    }
    period = rnd_chance(30) ? (int)rnd(1, n) : n;
    for (i = 0; i < period; i++) {
        c = rnd_char(alphabet);
        s[i] = c;
    }
    for (i = period; i < n; i++)
        s[i] = s[i - period];
    s[n] = '\0';

    len = n;
    if (shape == 7) {
        if (small)
            len = n == 1 || rnd_chance(50) ? n + 1 : n - 1;
        else
            len = n == 99 ? 100 : 99;
    }
    k = (int)rnd(0, n - 1);
    for (i = 0; i < len; i++)
        goal[i] = s[(k + i) % n];
    goal[len] = '\0';
    if (shape == 3) {
        at = rnd_chance(50) ? n - 1 : (int)rnd(0, n - 1);
        c = rnd_char(alphabet);
        goal[at] = c;
    } else if (shape == 4) {
        for (i = 0; i < n; i++)
            goal[i] = s[n - 1 - i];
    } else if (shape == 5) {
        for (i = n - 1; i > 0; i--) {
            j = (int)rnd(0, i);
            swap(i, j);
        }
    } else if (shape == 6) {
        for (i = 0; i < n; i++) {
            c = rnd_char(alphabet);
            goal[i] = c;
        }
    }
    put_string(s);
    put_string(goal);
}
