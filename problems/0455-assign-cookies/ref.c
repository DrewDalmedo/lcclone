/* Reference solution: sort both arrays. Go through the cookies from the
 * smallest up, giving each to the least demanding child not yet content
 * if it is big enough for that child, and setting it aside otherwise
 * (then it is too small for every child left). */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int findContentChildren(int *g, int gSize, int *s, int sSize)
{
    int child = 0, cookie;

    qsort(g, (size_t)gSize, sizeof *g, cmp_int);
    if (sSize > 0)
        qsort(s, (size_t)sSize, sizeof *s, cmp_int);
    for (cookie = 0; cookie < sSize && child < gSize; cookie++)
        if (s[cookie] >= g[child])
            child++;
    return child;
}
