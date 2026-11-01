/* arr is n distinct numbers; the pieces start as arr cut at random places,
 * which is a true case, and are then often changed: two numbers swapped
 * inside a piece, one number replaced by one that arr lacks, two numbers
 * swapped between pieces, the last number of a piece moved to the end of
 * another, a piece reversed, or arr's numbers shuffled and cut afresh.
 * Some of these give true, most false. The pieces are always given in a
 * random order. Small cases have 1 to 8 numbers, mostly from 1..12.
 * Full-size ones have 100 numbers, 1..100 each once, with the seed picking
 * the shape: random cuts, true; every piece of length 1, true; or random
 * cuts with the last two numbers of arr's last piece swapped, false only at
 * the very end of arr. A fourth shape has 99 numbers and ends arr's last
 * piece with the one number arr lacks. */
static int pc[100][100], len[100];

/* A random piece of length 2 or more, or -1 if there is none. */
static int long_piece(int m)
{
    int cand[100], c = 0, k;

    for (k = 0; k < m; k++)
        if (len[k] >= 2)
            cand[c++] = k;
    if (c == 0)
        return -1;
    k = (int)rnd(0, c - 1);
    return cand[k];
}

/* Cuts a[0..n) into pieces, cutting each gap with a chance of pct in 100;
 * returns the number of pieces. */
static int cut(const int *a, int n, int pct)
{
    int m = 0, i;

    len[0] = 0;
    for (i = 0; i < n; i++) {
        if (i > 0 && rnd_chance(pct))
            len[++m] = 0;
        pc[m][len[m]++] = a[i];
    }
    return m + 1;
}

static void put_pieces(int m)
{
    int ord[100], k, j;

    for (k = 0; k < m; k++)
        ord[k] = k;
    rnd_shuffle(ord, m);
    putchar('[');
    for (k = 0; k < m; k++) {
        printf("%s[", k ? "," : "");
        for (j = 0; j < len[ord[k]]; j++)
            printf("%s%d", j ? "," : "", pc[ord[k]][j]);
        putchar(']');
    }
    puts("]");
}

void gen(int small)
{
    int vals[100], arr[100];
    int top = 100, n = 100, shape, m, i, j, k, a, b, t;

    if (small) {
        n = (int)rnd(1, 8);
        top = rnd_chance(70) ? 12 : 100;
        shape = (int)rnd(0, 9);
    } else {
        shape = (int)rnd(0, 3);
        if (shape == 3)
            n = 99;
    }
    for (i = 0; i < top; i++)
        vals[i] = i + 1;
    rnd_shuffle(vals, top);
    for (i = 0; i < n; i++)
        arr[i] = vals[i];
    t = small ? (int)rnd(0, 100) : (int)rnd(5, 60);
    if (!small && shape == 1)
        t = 100;
    m = cut(arr, n, t);

    if (!small) {
        if (shape >= 2 && len[m - 1] == 1) {
            len[m - 2]++;
            pc[m - 2][len[m - 2] - 1] = pc[m - 1][0];
            m--;
        }
        k = m - 1;
        if (shape == 2) {
            t = pc[k][len[k] - 1];
            pc[k][len[k] - 1] = pc[k][len[k] - 2];
            pc[k][len[k] - 2] = t;
        } else if (shape == 3) {
            pc[k][len[k] - 1] = vals[99];
        }
    } else if (shape == 3 || shape == 4 || shape == 8) {
        k = long_piece(m);
        if (k >= 0 && shape != 8) {
            i = (int)rnd(0, len[k] - 2);
            j = (int)rnd(i + 1, len[k] - 1);
            t = pc[k][i];
            pc[k][i] = pc[k][j];
            pc[k][j] = t;
        } else if (k >= 0) {
            for (i = 0, j = len[k] - 1; i < j; i++, j--) {
                t = pc[k][i];
                pc[k][i] = pc[k][j];
                pc[k][j] = t;
            }
        }
    } else if (shape == 2 || shape == 5) {
        k = (int)rnd(0, m - 1);
        i = (int)rnd(0, len[k] - 1);
        j = (int)rnd(n, top - 1);
        pc[k][i] = vals[j];
    } else if (shape == 6 && m >= 2) {
        a = (int)rnd(0, m - 1);
        b = (int)rnd(0, m - 2);
        if (b >= a)
            b++;
        i = (int)rnd(0, len[a] - 1);
        j = (int)rnd(0, len[b] - 1);
        t = pc[a][i];
        pc[a][i] = pc[b][j];
        pc[b][j] = t;
    } else if (shape == 7 && m >= 2) {
        a = long_piece(m);
        if (a >= 0) {
            b = (int)rnd(0, m - 2);
            if (b >= a)
                b++;
            pc[b][len[b]++] = pc[a][--len[a]];
        }
    } else if (shape == 9) {
        rnd_shuffle(vals, n); /* vals[0..n) holds arr's numbers */
        t = (int)rnd(0, 100);
        m = cut(vals, n, t);
    }
    put_ints(arr, n);
    put_pieces(m);
}
