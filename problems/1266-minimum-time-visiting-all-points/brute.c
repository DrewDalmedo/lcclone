/* Slow and obviously right: for each leg, a breadth-first search over the
 * lattice points, using the eight king moves, finds the fewest moves from
 * one point to the next. The search stays inside the box around all the
 * points, widened by 1 on each side: pressing a path into the box (moving
 * each point of it to the nearest point of the box) never makes it longer,
 * so a shortest path inside the box exists. */
int minTimeToVisitAllPoints(int **points, int pointsSize,
                            int *pointsColSize)
{
    int lox = points[0][0], hix = lox, loy = points[0][1], hiy = loy;
    int w, h, total = 0, i, head, tail, x, y, dx, dy, cur, nx, ny;
    int *dist, *queue;

    (void)pointsColSize;
    for (i = 1; i < pointsSize; i++) {
        if (points[i][0] < lox) lox = points[i][0];
        if (points[i][0] > hix) hix = points[i][0];
        if (points[i][1] < loy) loy = points[i][1];
        if (points[i][1] > hiy) hiy = points[i][1];
    }
    lox--;
    loy--;
    w = hix + 1 - lox + 1;
    h = hiy + 1 - loy + 1;
    dist = malloc((size_t)w * h * sizeof *dist);
    queue = malloc((size_t)w * h * sizeof *queue);
    for (i = 1; i < pointsSize; i++) {
        int sx = points[i - 1][0] - lox, sy = points[i - 1][1] - loy;
        int tx = points[i][0] - lox, ty = points[i][1] - loy;

        for (cur = 0; cur < w * h; cur++)
            dist[cur] = -1;
        head = tail = 0;
        dist[sy * w + sx] = 0;
        queue[tail++] = sy * w + sx;
        while (dist[ty * w + tx] < 0) {
            cur = queue[head++];
            x = cur % w;
            y = cur / w;
            for (dx = -1; dx <= 1; dx++)
                for (dy = -1; dy <= 1; dy++) {
                    nx = x + dx;
                    ny = y + dy;
                    if (nx < 0 || nx >= w || ny < 0 || ny >= h
                        || dist[ny * w + nx] >= 0)
                        continue;
                    dist[ny * w + nx] = dist[cur] + 1;
                    queue[tail++] = ny * w + nx;
                }
        }
        total += dist[ty * w + tx];
    }
    free(dist);
    free(queue);
    return total;
}
