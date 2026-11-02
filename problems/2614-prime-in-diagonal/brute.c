/* Slow and obviously right: sieve of Eratosthenes up to the largest value
 * in the grid, then check every cell against the definition of a diagonal
 * cell and keep the largest prime among them. */
int diagonalPrime(int **nums, int numsSize, int *numsColSize)
{
    int n = numsSize, top = 1, best = 0, r, c, i, j;
    char *composite;

    (void)numsColSize;
    for (r = 0; r < n; r++)
        for (c = 0; c < n; c++)
            if (nums[r][c] > top)
                top = nums[r][c];
    composite = calloc((size_t)top + 1, 1);
    composite[0] = composite[1] = 1;
    for (i = 2; i <= top / i; i++)
        if (!composite[i])
            for (j = i * i; j <= top; j += i)
                composite[j] = 1;
    for (r = 0; r < n; r++)
        for (c = 0; c < n; c++)
            if ((r == c || r + c == n - 1) && !composite[nums[r][c]] &&
                nums[r][c] > best)
                best = nums[r][c];
    free(composite);
    return best;
}
