/* Reference solution: slide a window of k cells along blocks, keeping the
 * number of white cells inside it, and return the smallest such number. */
int minimumRecolors(char *blocks, int k)
{
    int n = (int)strlen(blocks), white = 0, best, i;

    for (i = 0; i < k; i++)
        white += blocks[i] == 'W';
    best = white;
    for (i = k; i < n; i++) {
        white += (blocks[i] == 'W') - (blocks[i - k] == 'W');
        if (white < best)
            best = white;
    }
    return best;
}
