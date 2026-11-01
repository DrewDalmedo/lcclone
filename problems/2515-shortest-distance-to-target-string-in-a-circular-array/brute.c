/* Slow and obviously right: walk clockwise from startIndex, one seat at a
 * time, for at most a full lap, and count the moves until a seat holds
 * target; do the same counterclockwise, and take the shorter walk. */
static int walk(char **words, int n, char *target, int start, int dir)
{
    int pos = start, moves;

    for (moves = 0; moves < n; moves++) {
        if (strcmp(words[pos], target) == 0)
            return moves;
        pos = (pos + dir + n) % n;
    }
    return -1;
}

int closestTarget(char **words, int wordsSize, char *target, int startIndex)
{
    int cw = walk(words, wordsSize, target, startIndex, 1);
    int ccw = walk(words, wordsSize, target, startIndex, -1);

    if (cw < 0)
        return -1;
    return cw < ccw ? cw : ccw;
}
