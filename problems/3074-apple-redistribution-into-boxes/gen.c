/* Box capacities first, then bags that fit in them. Small cases: up to 6
 * bags and 8 boxes with values up to 12, so that ties and exact fits are
 * common. Full-size cases have 50 bags and 50 boxes with values up to 50.
 * The seed picks the apple total: exactly the room of the k largest boxes
 * (an exact fit, answer k), exactly all the room (every box needed), or
 * random. */
static int cmp_down(const void *a, const void *b)
{
    return *(const int *)b - *(const int *)a;
}

void gen(int small)
{
    int capacity[50], apple[50], sorted[50];
    int m = small ? (int)rnd(1, 8) : 50;
    int top = small ? (int)rnd(1, 12) : 50;
    int shape = (int)rnd(0, 2);
    int n, room = 0, total, sum = 0, k, i;

    for (i = 0; i < m; i++) {
        capacity[i] = (int)rnd(1, top);
        room += capacity[i];
    }
    memcpy(sorted, capacity, (size_t)m * sizeof *sorted);
    qsort(sorted, (size_t)m, sizeof *sorted, cmp_down);
    if (shape == 0) {
        k = (int)rnd(1, m);
        total = 0;
        for (i = 0; i < k; i++)
            total += sorted[i];
    } else if (shape == 1) {
        total = room;
    } else {
        total = (int)rnd(1, room);
    }
    /* n bags of 1 to 50 apples adding up to total. */
    n = small ? (int)rnd(1, 6) : 50;
    if (n > total)
        n = total;
    while (50 * n < total)
        n++;
    for (i = 0; i < n; i++)
        apple[i] = 1;
    sum = n;
    while (sum < total) {
        i = (int)rnd(0, n - 1);
        k = 50 - apple[i] < total - sum ? 50 - apple[i] : total - sum;
        if (k > 0) {
            k = (int)rnd(1, k);
            apple[i] += k;
            sum += k;
        }
    }
    put_ints(apple, n);
    put_ints(capacity, m);
}
