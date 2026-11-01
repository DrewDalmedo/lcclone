/* Slow and obviously right: a breadth-first search over what is left of s,
 * written as a bit mask of the positions not yet erased. From each mask,
 * try every non-empty set of its positions whose letters spell a
 * palindrome, and erase it. The first time the empty mask is reached
 * gives the fewest moves. Exponential, so only for the small random
 * cases. */
static int spells_palindrome(const char *s, int n, unsigned sub)
{
    int pos[32], cnt = 0, i, j;

    for (i = 0; i < n; i++)
        if (sub >> i & 1u)
            pos[cnt++] = i;
    for (i = 0, j = cnt - 1; i < j; i++, j--)
        if (s[pos[i]] != s[pos[j]])
            return 0;
    return 1;
}

int removePalindromeSub(char *s)
{
    int n = (int)strlen(s), head = 0, tail = 0, answer = -1;
    unsigned full = (1u << n) - 1, mask, sub, next;
    int *dist = malloc(((size_t)full + 1) * sizeof *dist);
    unsigned *queue = malloc(((size_t)full + 1) * sizeof *queue);

    for (mask = 0; mask <= full; mask++)
        dist[mask] = -1;
    dist[full] = 0;
    queue[tail++] = full;
    while (head < tail) {
        mask = queue[head++];
        if (mask == 0) {
            answer = dist[mask];
            break;
        }
        for (sub = mask; sub != 0; sub = (sub - 1) & mask) {
            next = mask & ~sub;
            if (dist[next] < 0 && spells_palindrome(s, n, sub)) {
                dist[next] = dist[mask] + 1;
                queue[tail++] = next;
            }
        }
    }
    free(dist);
    free(queue);
    return answer;
}
