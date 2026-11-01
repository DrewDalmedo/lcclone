/* Slow and obviously right: the lines through the plots' four x and four
 * y coordinates cut the plane into a grid of cells, and the inside of each
 * cell lies either wholly inside a plot or wholly outside it. So the plots
 * overlap exactly when some cell of positive area lies inside both. Each
 * cell is tested by its centre, in doubled coordinates so that they stay
 * whole numbers. */
static void sort4(long long *v)
{
    long long t;
    int i, k;

    for (i = 1; i < 4; i++)
        for (k = i; k > 0 && v[k - 1] > v[k]; k--) {
            t = v[k];
            v[k] = v[k - 1];
            v[k - 1] = t;
        }
}

/* Whether the point (cx / 2, cy / 2) lies strictly inside rec. */
static bool inside(const int *rec, long long cx, long long cy)
{
    return 2LL * rec[0] < cx && cx < 2LL * rec[2] &&
           2LL * rec[1] < cy && cy < 2LL * rec[3];
}

bool isRectangleOverlap(int *rec1, int rec1Size, int *rec2, int rec2Size)
{
    long long xs[4], ys[4], cx, cy;
    int i, k;

    (void)rec1Size;
    (void)rec2Size;
    xs[0] = rec1[0];
    xs[1] = rec1[2];
    xs[2] = rec2[0];
    xs[3] = rec2[2];
    ys[0] = rec1[1];
    ys[1] = rec1[3];
    ys[2] = rec2[1];
    ys[3] = rec2[3];
    sort4(xs);
    sort4(ys);
    for (i = 0; i < 3; i++)
        for (k = 0; k < 3; k++) {
            if (xs[i] == xs[i + 1] || ys[k] == ys[k + 1])
                continue;
            cx = xs[i] + xs[i + 1];
            cy = ys[k] + ys[k + 1];
            if (inside(rec1, cx, cy) && inside(rec2, cx, cy))
                return true;
        }
    return false;
}
