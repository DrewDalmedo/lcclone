/* Slow and obviously right: keep the path from the top folder as a stack
 * of folder names, replaying the log entry by entry, then try "../" again
 * and again from where the walk ended, counting the steps until the stack
 * is empty. */
int minOperations(char **logs, int logsSize)
{
    char (*path)[10] = malloc((size_t)logsSize * sizeof *path);
    int top = 0, steps = 0, i, len;

    for (i = 0; i < logsSize; i++) {
        len = (int)strlen(logs[i]);
        if (len == 3 && logs[i][0] == '.' && logs[i][1] == '.')
            top = top > 0 ? top - 1 : 0;
        else if (len == 2 && logs[i][0] == '.')
            ;
        else {
            memcpy(path[top], logs[i], (size_t)len - 1);
            path[top][len - 1] = '\0';
            top++;
        }
    }
    while (top > 0) {
        top--;
        steps++;
    }
    free(path);
    return steps;
}
