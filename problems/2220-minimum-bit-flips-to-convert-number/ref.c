/* Reference solution: a switch must be flipped exactly when start and goal
 * disagree on it, and flipping any other one would have to be undone. The
 * disagreements are the 1 bits of start ^ goal: count them, dropping the
 * lowest set bit on each round (x &= x - 1). */
int minBitFlips(int start, int goal)
{
    unsigned x = (unsigned)start ^ (unsigned)goal;
    int moves = 0;

    for (; x != 0; x &= x - 1)
        moves++;
    return moves;
}
