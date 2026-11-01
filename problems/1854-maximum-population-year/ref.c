/* Reference solution: in a table for the years 1950..2050, add 1 at each
 * birth year and take 1 off at each death year. Adding the table up year
 * by year then gives each year's population; the first year where the
 * running total is largest is the answer. */
int maximumPopulation(int **logs, int logsSize, int *logsColSize)
{
    int change[101] = {0};
    int alive = 0, best = 0, year = 1950, i, y;

    (void)logsColSize;
    for (i = 0; i < logsSize; i++) {
        change[logs[i][0] - 1950]++;
        change[logs[i][1] - 1950]--;
    }
    for (y = 0; y < 101; y++) {
        alive += change[y];
        if (alive > best) {
            best = alive;
            year = 1950 + y;
        }
    }
    return year;
}
