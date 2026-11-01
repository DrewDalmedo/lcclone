/* Slow and obviously right: for each employee id from 0 up, look for a
 * task of theirs that no task in the log outlasts; the first id with one
 * is the answer. Every task's time is worked out afresh each time. */
static int task_time(int **logs, int i)
{
    return logs[i][1] - (i == 0 ? 0 : logs[i - 1][1]);
}

int hardestWorker(int n, int **logs, int logsSize, int *logsColSize)
{
    int id, i, k, longest;

    (void)logsColSize;
    for (id = 0; id < n; id++)
        for (i = 0; i < logsSize; i++) {
            if (logs[i][0] != id)
                continue;
            longest = 1;
            for (k = 0; k < logsSize; k++)
                if (task_time(logs, k) > task_time(logs, i))
                    longest = 0;
            if (longest)
                return id;
        }
    return -1;
}
