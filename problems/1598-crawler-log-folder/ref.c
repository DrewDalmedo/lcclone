/* Reference solution: only the depth below the top folder matters. "../"
 * takes one off it (never below 0), "./" leaves it alone, and anything
 * else adds one; the depth at the end is the number of "../" still
 * needed. */
int minOperations(char **logs, int logsSize)
{
    int depth = 0, i;

    for (i = 0; i < logsSize; i++) {
        if (strcmp(logs[i], "../") == 0) {
            if (depth > 0)
                depth--;
        } else if (strcmp(logs[i], "./") != 0) {
            depth++;
        }
    }
    return depth;
}
