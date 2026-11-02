/* Slow and obviously right: load containers one by one while there is a
 * free cell and the next one keeps the total within maxWeight. */
int maxContainers(int n, int w, int maxWeight)
{
    long long count = 0;

    while (count < (long long)n * n && (count + 1) * w <= maxWeight)
        count++;
    return (int)count;
}
