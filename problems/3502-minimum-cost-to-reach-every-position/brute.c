/* Slow and obviously right: search every arrangement of the queue, straight
 * from the rules. A state is who stands where (person n is you); from each
 * one you may trade with every other place, paying that person's fee when
 * they are ahead of you. Dijkstra's algorithm gives the cheapest way to
 * reach every state, and answer[i] is the cheapest state with you at place
 * i. Arrangements are numbered by their rank among all permutations, so
 * this only works for small queues: (n + 1)! states. */
typedef struct {
    int d, rank;
} brute_item;

static int brute_rank(const int *p, int m)
{
    int r = 0, i, j, smaller;

    for (i = 0; i < m; i++) {
        smaller = 0;
        for (j = i + 1; j < m; j++)
            if (p[j] < p[i])
                smaller++;
        r = r * (m - i) + smaller;
    }
    return r;
}

static void brute_unrank(int r, int *p, int m)
{
    int digit[16], used[16] = {0}, i, j, k;

    for (i = m - 1; i >= 0; i--) {
        digit[i] = r % (m - i);
        r /= m - i;
    }
    for (i = 0; i < m; i++) {
        for (j = 0, k = -1; j < m; j++)
            if (!used[j] && ++k == digit[i])
                break;
        used[j] = 1;
        p[i] = j;
    }
}

static void brute_push(brute_item *h, int *len, int d, int rank)
{
    int i = (*len)++;
    brute_item t;

    h[i].d = d;
    h[i].rank = rank;
    while (i > 0 && h[(i - 1) / 2].d > h[i].d) {
        t = h[i];
        h[i] = h[(i - 1) / 2];
        h[(i - 1) / 2] = t;
        i = (i - 1) / 2;
    }
}

static brute_item brute_pop(brute_item *h, int *len)
{
    brute_item top = h[0], t;
    int i = 0, c;

    h[0] = h[--*len];
    for (;;) {
        c = 2 * i + 1;
        if (c >= *len)
            break;
        if (c + 1 < *len && h[c + 1].d < h[c].d)
            c++;
        if (h[i].d <= h[c].d)
            break;
        t = h[i];
        h[i] = h[c];
        h[c] = t;
        i = c;
    }
    return top;
}

int *minCosts(int *cost, int costSize, int *returnSize)
{
    int n = costSize, m = n + 1, states = 1, len = 0, i, q, you, r, nd;
    int p[16], *dist, *answer;
    brute_item *heap, it;

    for (i = 2; i <= m; i++)
        states *= i;
    dist = malloc((size_t)states * sizeof *dist);
    heap = malloc((size_t)states * (size_t)m * sizeof *heap);
    answer = malloc((size_t)n * sizeof *answer);
    for (i = 0; i < states; i++)
        dist[i] = -1;
    for (i = 0; i < n; i++)
        answer[i] = -1;
    for (i = 0; i < m; i++)
        p[i] = i;
    r = brute_rank(p, m);
    dist[r] = 0;
    brute_push(heap, &len, 0, r);
    while (len > 0) {
        it = brute_pop(heap, &len);
        if (it.d != dist[it.rank])
            continue;
        brute_unrank(it.rank, p, m);
        for (you = 0; p[you] != n; you++)
            ;
        if (you < n && (answer[you] < 0 || it.d < answer[you]))
            answer[you] = it.d;
        for (q = 0; q < m; q++) {
            if (q == you)
                continue;
            nd = it.d + (q < you ? cost[p[q]] : 0);
            p[you] = p[q];
            p[q] = n;
            r = brute_rank(p, m);
            p[q] = p[you];
            p[you] = n;
            if (dist[r] < 0 || nd < dist[r]) {
                dist[r] = nd;
                brute_push(heap, &len, nd, r);
            }
        }
    }
    free(dist);
    free(heap);
    *returnSize = n;
    return answer;
}
