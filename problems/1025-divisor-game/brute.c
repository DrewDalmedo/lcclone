/* Slow and obviously right: work out every square from 1 up to n straight
 * from the rules. The player to move on square m wins when some divisor d
 * of m with d < m moves the marker to a square where the player to move
 * loses; on square 1 there is no move, so that player loses. */
bool divisorGame(int n)
{
    bool wins[1001];
    int m, d;

    wins[1] = false;
    for (m = 2; m <= n; m++) {
        wins[m] = false;
        for (d = 1; d < m; d++)
            if (m % d == 0 && !wins[m - d])
                wins[m] = true;
    }
    return wins[n];
}
