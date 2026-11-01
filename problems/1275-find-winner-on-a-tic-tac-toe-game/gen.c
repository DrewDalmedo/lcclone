/* A legal game, played until it is over or until it reaches a length the
 * seed picks. The seed picks how the players choose their squares:
 *   0-4: at random;
 *   5-9: sensibly: a player who can complete a line does, else blocks a
 *        line the other player is about to complete, else picks at random
 *        (games like that often end in a draw, or late).
 * Half the time the game runs until it is over; otherwise it stops after
 * 1 to 9 moves, or sooner if someone wins first. Every case is small, so
 * full-size cases are made the same way. */
static const int lines[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6},
    {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6},
};

/* The free square that completes a line of player p's, or -1. */
static int completes(const int *board, int p)
{
    int l, k, mine, free_sq;

    for (l = 0; l < 8; l++) {
        mine = 0;
        free_sq = -1;
        for (k = 0; k < 3; k++) {
            if (board[lines[l][k]] == p)
                mine++;
            else if (board[lines[l][k]] == 0)
                free_sq = lines[l][k];
        }
        if (mine == 2 && free_sq >= 0)
            return free_sq;
    }
    return -1;
}

static int won(const int *board, int p)
{
    int l;

    for (l = 0; l < 8; l++)
        if (board[lines[l][0]] == p && board[lines[l][1]] == p
            && board[lines[l][2]] == p)
            return 1;
    return 0;
}

void gen(int small)
{
    int board[9] = {0}, moves[18], n = 0, sq, p, k;
    int smart = (int)rnd(0, 9) >= 5;
    int stop = rnd_chance(50) ? 9 : (int)rnd(1, 9);

    (void)small;
    while (n < stop) {
        p = n % 2 == 0 ? 1 : 2;
        sq = -1;
        if (smart) {
            sq = completes(board, p);
            if (sq < 0)
                sq = completes(board, 3 - p);
        }
        if (sq < 0) {
            /* the k-th free square, k at random */
            k = (int)rnd(0, 8 - n);
            for (sq = 0; board[sq] != 0 || k > 0; sq++)
                if (board[sq] == 0)
                    k--;
        }
        board[sq] = p;
        moves[2 * n] = sq / 3;
        moves[2 * n + 1] = sq % 3;
        n++;
        if (won(board, p))
            break;
    }
    put_int_grid(moves, n, 2);
}
