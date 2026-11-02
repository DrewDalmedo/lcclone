/* Most cases are built around a random palindrome: left as it is, with
 * one extra letter inserted (anywhere, or near the far end), with two
 * extra letters, with two letters of different pairs changed, or as the
 * trap x y x P x y (or its reverse), where both letters at the first
 * mismatch equal the other one's neighbour but only one of the two
 * deletions works. The others are a run of one letter with two strays
 * near the middle (false at full size, after long checks), and, in small
 * cases only, random letters. Small cases have 1 to 10 letters from "pq"
 * or "pqr". Most short strings can be fixed, so the seed picks the answer
 * of a small case (false for an odd seed, true for an even one), and
 * shapes are drawn until one has it. Full-size ones have 10^5 letters,
 * and the seed picks the shape. */
enum { PALINDROME, ONE_EXTRA, TWO_EXTRA, TWO_CHANGED, TRAP, TRAP_REVERSED,
       STRAYS, RANDOM, LATE_EXTRA };

static void palindrome(char *p, int m, const char *letters)
{
    int i;

    for (i = 0; i < (m + 1) / 2; i++)
        p[i] = p[m - 1 - i] = rnd_char(letters);
}

/* Inserts c at index at of the m characters of p. */
static void insert(char *p, int m, int at, char c)
{
    memmove(p + at + 1, p + at, (size_t)(m - at));
    p[at] = c;
}

/* Changes one letter of the pair (i, n - 1 - i) of s, which matches, so
 * that the pair no longer does. */
static void unpair(char *s, int n, int i, const char *letters)
{
    int j = rnd_chance(50) ? i : n - 1 - i;
    char c;

    do
        c = rnd_char(letters);
    while (c == s[n - 1 - j]);
    s[j] = c;
}

static bool is_palindrome(const char *s, int i, int j)
{
    while (i < j)
        if (s[i++] != s[j--])
            return false;
    return true;
}

/* The answer for s[0..n), as ref.c works it out. */
static bool fits(const char *s, int n)
{
    int i = 0, j = n - 1;

    while (i < j && s[i] == s[j]) {
        i++;
        j--;
    }
    return i >= j || is_palindrome(s, i + 1, j) || is_palindrome(s, i, j - 1);
}

/* Fills s[0..n) with a string of the given shape. */
static void make(char *s, int n, int shape, const char *letters)
{
    int i, at, a, b;
    char c, x, y;

    if (shape == TWO_EXTRA && n < 2)
        shape = PALINDROME;
    if (shape == TWO_CHANGED && n < 4)
        shape = ONE_EXTRA;
    if ((shape == TRAP || shape == TRAP_REVERSED) && n < 6)
        shape = ONE_EXTRA;
    switch (shape) {
    case PALINDROME:
        palindrome(s, n, letters);
        break;
    case ONE_EXTRA:
        palindrome(s, n - 1, letters);
        at = (int)rnd(0, n - 1);
        c = rnd_char(letters);
        insert(s, n - 1, at, c);
        break;
    case LATE_EXTRA: /* the only deletion that works may be near the end */
        palindrome(s, n - 1, letters);
        at = n - 1 - (int)rnd(0, 5);
        c = rnd_char(letters);
        insert(s, n - 1, at, c);
        break;
    case TWO_EXTRA:
        palindrome(s, n - 2, letters);
        at = (int)rnd(0, n - 2);
        c = rnd_char(letters);
        insert(s, n - 2, at, c);
        at = (int)rnd(0, n - 1);
        c = rnd_char(letters);
        insert(s, n - 1, at, c);
        break;
    case TWO_CHANGED:
        palindrome(s, n, letters);
        a = (int)rnd(0, n / 2 - 2);
        b = (int)rnd(a + 1, n / 2 - 1);
        unpair(s, n, a, letters);
        unpair(s, n, b, letters);
        break;
    case TRAP:
    case TRAP_REVERSED:
        x = rnd_char(letters);
        do
            y = rnd_char(letters);
        while (y == x);
        s[0] = s[2] = s[n - 2] = x;
        s[1] = s[n - 1] = y;
        palindrome(s + 3, n - 5, letters);
        if (shape == TRAP_REVERSED)
            for (i = 0; i < n / 2; i++) {
                c = s[i];
                s[i] = s[n - 1 - i];
                s[n - 1 - i] = c;
            }
        break;
    case STRAYS:
        /* all one letter, with another one 1 to 3 places before the middle
         * and a second 2 to 4 places further from it on the other side, so
         * that at full size no single deletion helps */
        for (i = 0; i < n; i++)
            s[i] = letters[0];
        a = (int)rnd(1, 3);
        b = (int)rnd(2, 4);
        b += a;
        a = (n - 1) / 2 - a;
        b = n / 2 + b;
        s[a < 0 ? 0 : a] = letters[1];
        s[b > n - 1 ? n - 1 : b] = letters[1];
        break;
    default:
        for (i = 0; i < n; i++)
            s[i] = rnd_char(letters);
        break;
    }
    s[n] = '\0';
}

void gen(int small)
{
    static const int small_shapes[] = { PALINDROME, ONE_EXTRA, ONE_EXTRA,
        TWO_EXTRA, TWO_EXTRA, TWO_CHANGED, TWO_CHANGED, TRAP, TRAP_REVERSED,
        STRAYS, RANDOM, RANDOM };
    static const int large_shapes[] = { PALINDROME, ONE_EXTRA, TWO_EXTRA,
        TRAP, TRAP_REVERSED, STRAYS, LATE_EXTRA };
    int n, shape, want, tries;
    const char *letters;
    char *s;

    if (small) {
        s = malloc(11);
        want = gen_seed() % 2 == 0;
        for (tries = 0; tries < 200; tries++) {
            n = (int)rnd(1, 10);
            letters = rnd_chance(30) ? "pq" : "pqr";
            shape = small_shapes[rnd(0, 11)];
            make(s, n, shape, letters);
            if (fits(s, n) == want)
                break;
        }
    } else {
        n = 100000;
        s = malloc((size_t)n + 1);
        make(s, n, large_shapes[gen_seed() % 7],
             "abcdefghijklmnopqrstuvwxyz");
    }
    put_string(s);
    free(s);
}
