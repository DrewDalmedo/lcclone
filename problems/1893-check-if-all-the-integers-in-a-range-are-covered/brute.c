/* Slow and obviously right: for each post from left to right, look through
 * every stretch for one that includes it. */
bool isCovered(int **ranges, int rangesSize, int *rangesColSize, int left,
               int right)
{
    int x, i;
    bool painted;

    (void)rangesColSize;
    for (x = left; x <= right; x++) {
        painted = false;
        for (i = 0; i < rangesSize; i++)
            if (ranges[i][0] <= x && x <= ranges[i][1])
                painted = true;
        if (!painted)
            return false;
    }
    return true;
}
