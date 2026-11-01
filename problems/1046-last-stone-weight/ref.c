/* Reference solution: keep the stones in a binary max-heap. Take out the
 * two heaviest, put back their difference if it isn't 0, and repeat until
 * fewer than two stones are left. */
static void sift_down(int *h, int n, int i)
{
    int c, t;

    while ((c = 2 * i + 1) < n) {
        if (c + 1 < n && h[c + 1] > h[c])
            c++;
        if (h[i] >= h[c])
            break;
        t = h[i];
        h[i] = h[c];
        h[c] = t;
        i = c;
    }
}

static void sift_up(int *h, int i)
{
    int p, t;

    while (i > 0 && h[p = (i - 1) / 2] < h[i]) {
        t = h[i];
        h[i] = h[p];
        h[p] = t;
        i = p;
    }
}

/* Takes the largest stone out of the heap h of n stones. */
static int pop(int *h, int n)
{
    int top = h[0];

    h[0] = h[n - 1];
    sift_down(h, n - 1, 0);
    return top;
}

int lastStoneWeight(int *stones, int stonesSize)
{
    int *h = malloc((size_t)stonesSize * sizeof *h);
    int n = stonesSize, i, a, b, last;

    memcpy(h, stones, (size_t)n * sizeof *h);
    for (i = n / 2 - 1; i >= 0; i--)
        sift_down(h, n, i);
    while (n >= 2) {
        a = pop(h, n--);
        b = pop(h, n--);
        if (a > b) {
            h[n] = a - b;
            sift_up(h, n++);
        }
    }
    last = n == 1 ? h[0] : 0;
    free(h);
    return last;
}
