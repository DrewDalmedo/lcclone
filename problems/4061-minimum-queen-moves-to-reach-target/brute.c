/* Slow and obviously right: breadth-first search over the 64 squares,
 * trying every queen move (each of the eight directions, every length
 * that stays on the board) from each square reached. */
int minQueenMoves(int *source, int sourceSize, int *target, int targetSize)
{
    static const int step[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1},
                                   {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    int dist[9][9], queue[64][2], head = 0, tail = 0, r, c, d, k, nr, nc;

    (void)sourceSize;
    (void)targetSize;
    for (r = 1; r <= 8; r++)
        for (c = 1; c <= 8; c++)
            dist[r][c] = -1;
    dist[source[0]][source[1]] = 0;
    queue[tail][0] = source[0];
    queue[tail][1] = source[1];
    tail++;
    while (head < tail) {
        r = queue[head][0];
        c = queue[head][1];
        head++;
        for (d = 0; d < 8; d++)
            for (k = 1; k <= 7; k++) {
                nr = r + k * step[d][0];
                nc = c + k * step[d][1];
                if (nr < 1 || nr > 8 || nc < 1 || nc > 8)
                    break;
                if (dist[nr][nc] < 0) {
                    dist[nr][nc] = dist[r][c] + 1;
                    queue[tail][0] = nr;
                    queue[tail][1] = nc;
                    tail++;
                }
            }
    }
    return dist[target[0]][target[1]];
}
