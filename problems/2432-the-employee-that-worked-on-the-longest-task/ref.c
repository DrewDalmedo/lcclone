/* Reference solution: one pass over the log, timing each task from the
 * previous finish (0 for the first). Keep the best task so far, replacing
 * it with one that is longer, or as long but done by a smaller id. */
int hardestWorker(int n, int **logs, int logsSize, int *logsColSize)
{
    int prev = 0, best_len = 0, best_id = n, len, i;

    (void)logsColSize;
    for (i = 0; i < logsSize; i++) {
        len = logs[i][1] - prev;
        if (len > best_len || (len == best_len && logs[i][0] < best_id)) {
            best_len = len;
            best_id = logs[i][0];
        }
        prev = logs[i][1];
    }
    return best_id;
}
