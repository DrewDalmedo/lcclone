/* Slow and obviously right: explore every number reachable from num by
 * single trades of two same-family digits (a breadth-first search over
 * digit strings, remembering the ones seen in a hash table), and keep the
 * largest. */
struct seen {
    long long key;
    UT_hash_handle hh;
};

int largestInteger(int num)
{
    char start[16], cur[16];
    long long *queue, best = num, v;
    struct seen *table = NULL, *items, *s;
    /* At most 22680 numbers are reachable (nine digits of one family,
     * making four pairs and a single), so cap covers every allowed num,
     * not only the small ones. */
    int n, head = 0, tail = 0, cap = 25000, i, k;
    char t;

    n = snprintf(start, sizeof start, "%d", num);
    queue = malloc((size_t)cap * sizeof *queue);
    items = malloc((size_t)cap * sizeof *items);
    items[0].key = num;
    HASH_ADD(hh, table, key, sizeof(long long), &items[0]);
    queue[tail++] = num;
    while (head < tail) {
        snprintf(cur, sizeof cur, "%0*lld", n, queue[head++]);
        for (i = 0; i < n; i++)
            for (k = i + 1; k < n; k++) {
                if ((cur[i] - '0') % 2 != (cur[k] - '0') % 2)
                    continue;
                t = cur[i];
                cur[i] = cur[k];
                cur[k] = t;
                v = atoll(cur);
                HASH_FIND(hh, table, &v, sizeof(long long), s);
                if (!s) {
                    items[tail].key = v;
                    HASH_ADD(hh, table, key, sizeof(long long), &items[tail]);
                    queue[tail++] = v;
                    if (v > best)
                        best = v;
                }
                cur[k] = cur[i];
                cur[i] = t;
            }
    }
    HASH_CLEAR(hh, table);
    free(items);
    free(queue);
    return (int)best;
}
