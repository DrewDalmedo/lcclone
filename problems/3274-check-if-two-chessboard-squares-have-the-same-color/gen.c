/* Two random squares. About one case in six names the same square twice,
 * and one in four puts the second square right next to the first, along
 * an edge or a diagonal. The board is tiny, so small and full-size cases
 * are made alike. */
void gen(int small)
{
    char a[3], b[3];
    int c1, r1, c2, r2, shape, dc, dr;

    (void)small;
    c1 = (int)rnd(0, 7);
    r1 = (int)rnd(0, 7);
    shape = (int)rnd(0, 11);
    if (shape < 2) {
        c2 = c1;
        r2 = r1;
    } else if (shape < 5) {
        do {
            dc = (int)rnd(-1, 1);
            dr = (int)rnd(-1, 1);
        } while ((dc == 0 && dr == 0) || c1 + dc < 0 || c1 + dc > 7 ||
                 r1 + dr < 0 || r1 + dr > 7);
        c2 = c1 + dc;
        r2 = r1 + dr;
    } else {
        c2 = (int)rnd(0, 7);
        r2 = (int)rnd(0, 7);
    }
    a[0] = (char)('a' + c1);
    a[1] = (char)('1' + r1);
    a[2] = '\0';
    b[0] = (char)('a' + c2);
    b[1] = (char)('1' + r2);
    b[2] = '\0';
    put_string(a);
    put_string(b);
}
