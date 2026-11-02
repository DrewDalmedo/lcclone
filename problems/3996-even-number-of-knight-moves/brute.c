/* Slow and obviously right: a breadth-first search over the states
 * (square, number of jumps so far is even or odd), starting from start
 * after zero jumps, and following every jump that stays on the board. */
bool canReach(int *start, int startSize, int *target, int targetSize)
{
    static const int dx[8] = {1, 2, 2, 1, -1, -2, -2, -1};
    static const int dy[8] = {2, 1, -1, -2, -2, -1, 1, 2};
    bool seen[8][8][2];
    int queue[128][3], head = 0, tail = 0, k;

    (void)startSize;
    (void)targetSize;
    memset(seen, 0, sizeof seen);
    seen[start[0]][start[1]][0] = true;
    queue[tail][0] = start[0];
    queue[tail][1] = start[1];
    queue[tail][2] = 0;
    tail++;
    while (head < tail) {
        int x = queue[head][0], y = queue[head][1], odd = queue[head][2];

        head++;
        for (k = 0; k < 8; k++) {
            int nx = x + dx[k], ny = y + dy[k];

            if (nx < 0 || nx > 7 || ny < 0 || ny > 7 || seen[nx][ny][!odd])
                continue;
            seen[nx][ny][!odd] = true;
            queue[tail][0] = nx;
            queue[tail][1] = ny;
            queue[tail][2] = !odd;
            tail++;
        }
    }
    return seen[target[0]][target[1]][0];
}
