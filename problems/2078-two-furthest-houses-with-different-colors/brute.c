/* Slow and obviously right: try every pair of houses. */
int maxDistance(int *colors, int colorsSize)
{
    int best = 0, i, j;

    for (i = 0; i < colorsSize; i++)
        for (j = i + 1; j < colorsSize; j++)
            if (colors[i] != colors[j] && j - i > best)
                best = j - i;
    return best;
}
