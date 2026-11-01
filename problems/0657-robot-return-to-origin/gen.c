/* A list of moves. Most cases start from a walk that returns to (0, 0): a
 * shuffle of as many 'U' as 'D' and as many 'R' as 'L'. The seed then
 * leaves it as it is, changes one move to another letter, or, at full size,
 * swaps letters so that one axis is off by 256 or the up moves match the
 * left ones instead of the down ones. Small cases have up to 12 moves.
 * Full-size ones have 2 * 10^4 moves (even seeds) or a random number, and
 * also come as a long excursion (all the up moves, then right, then down,
 * then left), as random letters, or as a walk that ends one step from
 * (0, 0) along one axis and w steps along the other, so that a single
 * counter such as x + w * y or w * x + y reads 0 (w up to 10^4). */
static char moves[20001];

static void shuffle_chars(char *s, int n)
{
    int i, j;
    char c;

    for (i = n - 1; i > 0; i--) {
        j = (int)rnd(0, i);
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* Fills moves[0..n) with a walk back to (0, 0) when n is even, or one with
 * a single extra move when it is odd; u is the number of 'U' moves. */
static void balanced(int n, int u)
{
    int i = 0, k, r = n / 2 - u;

    for (k = 0; k < u; k++) {
        moves[i++] = 'U';
        moves[i++] = 'D';
    }
    for (k = 0; k < r; k++) {
        moves[i++] = 'R';
        moves[i++] = 'L';
    }
    if (i < n)
        moves[i++] = rnd_char("UDLR");
}

void gen(int small)
{
    int n, u, shape, i, k;

    if (small) {
        n = (int)rnd(1, 12);
        if (rnd_chance(70) && n % 2 == 1)
            n = n == 1 ? 2 : n - 1;
        u = (int)rnd(0, n / 2);
        balanced(n, u);
        shuffle_chars(moves, n);
        if (rnd_chance(30)) {
            i = (int)rnd(0, n - 1);
            moves[i] = rnd_char("UDLR");
        }
        moves[n] = '\0';
        put_string(moves);
        return;
    }
    n = gen_seed() % 2 == 0 ? 20000 : 2 * (int)rnd(500, 10000);
    shape = (int)rnd(0, 6);
    if (shape == 6) {
        static const int weights[] = {2, 3, 10, 100, 256, 1000, 10000};
        int w, m;
        char one, many;

        do
            w = weights[rnd(0, 6)];
        while (w + 1 > n);
        if (rnd_chance(50)) {
            /* (1, -w) or (-1, w): w * x + y is 0 */
            one = rnd_chance(50) ? 'R' : 'L';
            many = one == 'R' ? 'D' : 'U';
        } else {
            /* (-w, 1) or (w, -1): x + w * y is 0 */
            one = rnd_chance(50) ? 'U' : 'D';
            many = one == 'U' ? 'L' : 'R';
        }
        m = n - (w + 1);
        if (m % 2 == 1)
            m--;
        balanced(m, (int)rnd(0, m / 2));
        moves[m] = one;
        for (i = 0; i < w; i++)
            moves[m + 1 + i] = many;
        n = m + w + 1;
        shuffle_chars(moves, n);
        moves[n] = '\0';
        put_string(moves);
        return;
    }
    if (shape == 5) {
        for (i = 0; i < n; i++)
            moves[i] = rnd_char("UDLR");
        moves[n] = '\0';
        put_string(moves);
        return;
    }
    u = (int)rnd(200, n / 2 - 200);
    balanced(n, u);
    if (shape == 4) {
        /* an excursion: up, right, down, left, in four long legs */
        k = 0;
        for (i = 0; i < u; i++)
            moves[k++] = 'U';
        for (i = 0; i < n / 2 - u; i++)
            moves[k++] = 'R';
        for (i = 0; i < u; i++)
            moves[k++] = 'D';
        for (i = 0; i < n / 2 - u; i++)
            moves[k++] = 'L';
    } else {
        shuffle_chars(moves, n);
    }
    if (shape == 1) {
        /* one move changed, on the last step or anywhere */
        i = rnd_chance(50) ? n - 1 : (int)rnd(0, n - 1);
        moves[i] = moves[i] == 'U' ? 'D' : moves[i] == 'D' ? 'R'
                 : moves[i] == 'R' ? 'L' : 'U';
    } else if (shape == 2) {
        /* 128 'L' become 'R': right is 256 ahead of left */
        for (i = 0, k = 0; i < n && k < 128; i++)
            if (moves[i] == 'L') {
                moves[i] = 'R';
                k++;
            }
    } else if (shape == 3) {
        /* every 'D' becomes 'L' and every 'L' a 'D': as many 'U' as 'L',
         * and as many 'R' as 'D' */
        for (i = 0; i < n; i++)
            if (moves[i] == 'D')
                moves[i] = 'L';
            else if (moves[i] == 'L')
                moves[i] = 'D';
    }
    moves[n] = '\0';
    put_string(moves);
}
