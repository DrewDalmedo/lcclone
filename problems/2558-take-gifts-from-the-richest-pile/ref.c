/* Reference solution: keep the piles in a max-heap. Each round pops the
 * largest pile, pushes back its integer square root, and the total is
 * summed in 64 bits at the end. */
static int isqrt_int(int c)
{
    long long r = (long long)sqrt((double)c);

    while (r * r > c)
        r--;
    while ((r + 1) * (r + 1) <= c)
        r++;
    return (int)r;
}

static void sift_down(int *h, int n, int i)
{
    int child, t;

    for (;;) {
        child = 2 * i + 1;
        if (child >= n)
            break;
        if (child + 1 < n && h[child + 1] > h[child])
            child++;
        if (h[child] <= h[i])
            break;
        t = h[i];
        h[i] = h[child];
        h[child] = t;
        i = child;
    }
}

long long pickGifts(int *gifts, int giftsSize, int k)
{
    int *h = malloc((size_t)giftsSize * sizeof *h);
    long long total = 0;
    int i;

    memcpy(h, gifts, (size_t)giftsSize * sizeof *h);
    for (i = giftsSize / 2 - 1; i >= 0; i--)
        sift_down(h, giftsSize, i);
    for (i = 0; i < k && h[0] > 1; i++) {
        h[0] = isqrt_int(h[0]);
        sift_down(h, giftsSize, 0);
    }
    for (i = 0; i < giftsSize; i++)
        total += h[i];
    free(h);
    return total;
}
