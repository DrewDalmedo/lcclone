/* Reference solution: compare the two distances to z. */
int findClosest(int x, int y, int z)
{
    int d1 = abs(x - z), d2 = abs(y - z);

    if (d1 < d2)
        return 1;
    if (d2 < d1)
        return 2;
    return 0;
}
