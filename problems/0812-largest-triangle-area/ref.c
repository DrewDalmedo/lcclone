/* Reference solution: a largest triangle can always have its corners on
 * the convex hull of the points, so build the hull (Andrew's monotone
 * chain) and try every triple of its corners. Twice a triangle's area is
 * the absolute value of an integer cross product, halved at the end. */
struct pt {
    int x, y;
};

static int pt_cmp(const void *a, const void *b)
{
    const struct pt *p = a, *q = b;

    if (p->x != q->x)
        return p->x < q->x ? -1 : 1;
    return (p->y > q->y) - (p->y < q->y);
}

/* Twice the signed area of o, a, b: positive when they turn left. */
static int pt_cross(struct pt o, struct pt a, struct pt b)
{
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

double largestTriangleArea(int **points, int pointsSize, int *pointsColSize)
{
    int n = pointsSize, k = 0, lower, best = 0, i, j, m;
    struct pt *p = malloc((size_t)n * sizeof *p);
    struct pt *h = malloc((size_t)(2 * n) * sizeof *h);

    (void)pointsColSize;
    for (i = 0; i < n; i++) {
        p[i].x = points[i][0];
        p[i].y = points[i][1];
    }
    qsort(p, (size_t)n, sizeof *p, pt_cmp);
    /* The lower hull from left to right, then the upper one back; points
     * on a hull edge are dropped. */
    for (i = 0; i < n; i++) {
        while (k >= 2 && pt_cross(h[k - 2], h[k - 1], p[i]) <= 0)
            k--;
        h[k++] = p[i];
    }
    lower = k + 1;
    for (i = n - 2; i >= 0; i--) {
        while (k >= lower && pt_cross(h[k - 2], h[k - 1], p[i]) <= 0)
            k--;
        h[k++] = p[i];
    }
    k--; /* the last corner is the first one again */
    for (i = 0; i < k; i++)
        for (j = i + 1; j < k; j++)
            for (m = j + 1; m < k; m++) {
                int a = abs(pt_cross(h[i], h[j], h[m]));

                if (a > best)
                    best = a;
            }
    free(p);
    free(h);
    return best / 2.0;
}
