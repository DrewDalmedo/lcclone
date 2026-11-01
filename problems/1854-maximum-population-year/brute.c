/* Slow and obviously right: for every year from 1950 to 2050, count the
 * rows with b <= year < d, and keep the first year with the largest
 * count. */
int maximumPopulation(int **logs, int logsSize, int *logsColSize)
{
    int best = -1, answer = 0, year, count, i;

    (void)logsColSize;
    for (year = 1950; year <= 2050; year++) {
        count = 0;
        for (i = 0; i < logsSize; i++)
            if (logs[i][0] <= year && year < logs[i][1])
                count++;
        if (count > best) {
            best = count;
            answer = year;
        }
    }
    return answer;
}
