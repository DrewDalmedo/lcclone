/* Reference solution: after k ticks, column j of an even row holds the
 * value that was in column (j + k) mod n, and column j of an odd row the
 * value that was in column (j - k) mod n. Compare every position with the
 * one its value came from. Reducing k mod n first keeps the indices from
 * going negative. */
bool areSimilar(int **mat, int matSize, int *matColSize, int k)
{
    int n = matColSize[0], s = k % n, i, j, from;

    for (i = 0; i < matSize; i++)
        for (j = 0; j < n; j++) {
            from = i % 2 == 0 ? (j + s) % n : (j + n - s) % n;
            if (mat[i][j] != mat[i][from])
                return false;
        }
    return true;
}
