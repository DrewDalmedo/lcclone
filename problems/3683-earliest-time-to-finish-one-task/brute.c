/* Slow and obviously right: let the clock run from time 0 and stop at the
 * first time at which some task, started at s and running for t, is
 * complete. */
int earliestTime(int **tasks, int tasksSize, int *tasksColSize)
{
    int time, i;

    (void)tasksColSize;
    for (time = 0;; time++)
        for (i = 0; i < tasksSize; i++)
            if (tasks[i][0] + tasks[i][1] == time)
                return time;
}
