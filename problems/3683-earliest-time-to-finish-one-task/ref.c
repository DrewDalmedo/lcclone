/* Reference solution: one pass over the rows, keeping the smallest
 * completion time s + t, starting from the first row's. */
int earliestTime(int **tasks, int tasksSize, int *tasksColSize)
{
    int best = tasks[0][0] + tasks[0][1], i;

    (void)tasksColSize;
    for (i = 1; i < tasksSize; i++)
        if (tasks[i][0] + tasks[i][1] < best)
            best = tasks[i][0] + tasks[i][1];
    return best;
}
