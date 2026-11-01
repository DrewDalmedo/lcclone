/* Slow and obviously right: test every number of the range. */
int countOdds(int low, int high)
{
    int count = 0;
    long long x;

    for (x = low; x <= high; x++)
        if (x % 2 == 1)
            count++;
    return count;
}
