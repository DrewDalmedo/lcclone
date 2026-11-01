/* Builds the strings column by column. Each column is random, ordered
 * (sorted random letters, ties included), constant, or ordered but for a
 * single drop at one pair of neighbours.
 *
 * Small cases: 1 to 6 strings of 1 to 6 letters, mostly from "abc" so
 * that ties are common; the seed picks each column's kind.
 *
 * Full-size cases: 100 strings of 1000 letters, in a shape the seed picks:
 *   0. random letters (almost every column is out of order);
 *   1. ordered columns, about half of them with one drop, at the bottom
 *      pair half the time;
 *   2. every column ordered, so the answer is 0 and every pair must be
 *      looked at. */
static char grid[100][1001];

/* Sorts col[0..n) by counting the letters. */
static void sort_letters(char *col, int n)
{
    int count[26] = {0}, i, c, k = 0;

    for (i = 0; i < n; i++)
        count[col[i] - 'a']++;
    for (c = 0; c < 26; c++)
        for (i = 0; i < count[c]; i++)
            col[k++] = (char)('a' + c);
}

/* In an ordered column, makes the pair (i, i + 1) the only drop. */
static void make_drop(char *col, int i)
{
    char t;

    if (col[i] < col[i + 1]) {
        t = col[i];
        col[i] = col[i + 1];
        col[i + 1] = t;
    } else if (col[i] > 'a') {
        col[i + 1] = (char)(col[i] - 1);
    } else {
        col[i] = 'b';
    }
}

void gen(int small)
{
    static const char *const alphabets[] = {
        "abc", "ab", "abcdefghijklmnopqrstuvwxyz",
    };
    char *rows[100], col[100];
    int n = small ? (int)rnd(1, 6) : 100, m = small ? (int)rnd(1, 6) : 1000;
    int shape = small ? -1 : (int)(gen_seed() % 3);
    int i, j, kind, at, pick = small ? (int)rnd(0, 2) : 2;
    const char *letters = alphabets[pick];

    for (j = 0; j < m; j++) {
        if (small)
            kind = (int)rnd(0, 3);
        else if (shape == 0)
            kind = 0;
        else if (shape == 1)
            kind = rnd_chance(50) ? 3 : 1;
        else
            kind = 1;
        for (i = 0; i < n; i++)
            col[i] = rnd_char(letters);
        if (kind == 2) {
            for (i = 1; i < n; i++)
                col[i] = col[0];
        } else if (kind >= 1) {
            sort_letters(col, n);
        }
        if (kind == 3 && n > 1) {
            at = rnd_chance(50) ? n - 2 : (int)rnd(0, n - 2);
            make_drop(col, at);
        }
        for (i = 0; i < n; i++)
            grid[i][j] = col[i];
    }
    for (i = 0; i < n; i++) {
        grid[i][m] = '\0';
        rows[i] = grid[i];
    }
    put_strings(rows, n);
}
