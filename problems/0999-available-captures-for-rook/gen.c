/* One 8 x 8 board. The rook stands on a random square, on an edge or in a
 * corner more often than by chance; every other square is empty, a bishop
 * or a pawn. The share of empty squares and the mix of bishops and pawns
 * change from case to case, so that the rook's rays are sometimes crowded
 * and sometimes empty up to the edge. The board is always 8 x 8, so small
 * and full-size cases are made alike. */
void gen(int small)
{
    char board[64];
    int empty = (int)rnd(30, 95);   /* percent of squares left empty */
    int bishops = (int)rnd(0, 100); /* percent of pieces that are bishops */
    int r, c, i;

    (void)small;
    for (i = 0; i < 64; i++) {
        if (rnd_chance(empty))
            board[i] = '.';
        else if (rnd_chance(bishops))
            board[i] = 'B';
        else
            board[i] = 'p';
    }
    r = (int)rnd(0, 7);
    c = (int)rnd(0, 7);
    if (rnd_chance(25))
        r = rnd_chance(50) ? 0 : 7;
    if (rnd_chance(25))
        c = rnd_chance(50) ? 0 : 7;
    board[r * 8 + c] = 'R';
    put_char_grid(board, 8, 8);
}
