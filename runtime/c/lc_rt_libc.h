/*
 * lc_rt_libc.h: lc's own versions of a few C library functions. lc puts
 * this file between the prelude and the solution in every C build, and
 * at its end makes each of those names a macro for lc's version, so that
 * a solution, and the runtime after it, call lc's. LeetCode's C library is
 * glibc; with these, a solution behaves as there on every system:
 *
 * - qsort is glibc's merge sort. It makes the same comparisons in the
 *   same order, so it gives the same result, also with a comparator that
 *   only returns 0 or 1. It is stable: elements that compare equal keep
 *   their order. macOS's and musl's qsort are neither.
 * - realloc and free note when the solution gives back an array that the
 *   harness handed it (lc_rt_watch), so that the harness then leaves that
 *   array alone (lc_rt.c). The solution may do that, as on LeetCode: say,
 *   realloc an input array to twice its size, fill it and return it.
 *
 * Like the rest of the runtime, every name here is static and starts with
 * lc_rt_, and the macros are object-like, so they also rename a pointer to
 * the function, such as `void (*f)(void *) = free;`. clangd doesn't read
 * this file: to an editor, these are the C library's functions, which have
 * the same types.
 */

#if defined(__GNUC__) || defined(__clang__)
#define LC_RT_LIBC static __attribute__((unused))
#else
#define LC_RT_LIBC static
#endif

/*
 * clang's -fsanitize=function checks a call through a pointer against the
 * type of the function it reaches. The C library's qsort isn't built with
 * it, so on LeetCode a comparator declared as int cmp(int *, int *) and
 * cast to qsort's type is called without complaint; lc's sort mustn't
 * complain either. (gcc has no such check.)
 */
#if defined(__clang__)
#define LC_RT_ANY_CMP __attribute__((no_sanitize("function")))
#else
#define LC_RT_ANY_CMP
#endif

/* ------------------------------------------------------------------ */
/* qsort                                                               */
/* ------------------------------------------------------------------ */

/* Copies one element; the common sizes as fixed-size copies, which the
 * compiler turns into a load and a store. */
LC_RT_LIBC void lc_rt_copy(char *to, const char *from, size_t size)
{
    if (size == 4)
        memcpy(to, from, 4);
    else if (size == 8)
        memcpy(to, from, 8);
    else
        memcpy(to, from, size);
}

/*
 * glibc's msort_with_tmp: sorts the n elements of `size` bytes at b,
 * using tmp, which holds n of them. The first half is the first n / 2
 * elements, and a merge takes from the first half while cmp says <= 0.
 */
LC_RT_LIBC LC_RT_ANY_CMP void lc_rt_msort(char *b, size_t n, size_t size,
                                          int (*cmp)(const void *,
                                                     const void *),
                                          char *tmp)
{
    size_t n1, n2;
    char *b1, *b2, *t = tmp;

    if (n <= 1)
        return;
    n1 = n / 2;
    n2 = n - n1;
    b1 = b;
    b2 = b + n1 * size;
    lc_rt_msort(b1, n1, size, cmp, tmp);
    lc_rt_msort(b2, n2, size, cmp, tmp);
    while (n1 > 0 && n2 > 0) {
        if (cmp(b1, b2) <= 0) {
            lc_rt_copy(t, b1, size);
            b1 += size;
            n1--;
        } else {
            lc_rt_copy(t, b2, size);
            b2 += size;
            n2--;
        }
        t += size;
    }
    if (n1 > 0)
        memcpy(t, b1, n1 * size);
    memcpy(b, tmp, (n - n2) * size);
}

LC_RT_LIBC void lc_rt_qsort(void *base, size_t n, size_t size,
                            int (*cmp)(const void *, const void *))
{
    char buf[1024], *tmp = buf;

    if (n <= 1)
        return;
    /* As glibc, small sorts use the stack, and when there is no memory
     * for the copy the sort is done in place (by the C library here). */
    if (size > 0 && n > (size_t)-1 / size) {
        qsort(base, n, size, cmp);
        return;
    }
    if (n * size > sizeof buf) {
        tmp = (char *)malloc(n * size);
        if (!tmp) {
            qsort(base, n, size, cmp);
            return;
        }
    }
    lc_rt_msort((char *)base, n, size, cmp, tmp);
    if (tmp != buf)
        free(tmp);
}

/* ------------------------------------------------------------------ */
/* realloc and free                                                    */
/* ------------------------------------------------------------------ */

/* The arrays the harness handed the solution, by address, and whether
 * the solution gave each back. One per array parameter; past the last
 * slot, arrays aren't watched. */
#define LC_RT_WATCH_MAX 64
static uintptr_t lc_rt_watched[LC_RT_WATCH_MAX];
static unsigned char lc_rt_given_back[LC_RT_WATCH_MAX];
static int lc_rt_nwatched;

LC_RT_LIBC void lc_rt_watch(const void *p)
{
    if (lc_rt_nwatched < LC_RT_WATCH_MAX)
        lc_rt_watched[lc_rt_nwatched++] = (uintptr_t)p;
}

/* 1 if p is an array the harness watches and the solution freed or
 * reallocated. */
LC_RT_LIBC int lc_rt_released(const void *p)
{
    int i;

    for (i = 0; i < lc_rt_nwatched; i++)
        if (lc_rt_watched[i] == (uintptr_t)p)
            return lc_rt_given_back[i];
    return 0;
}

LC_RT_LIBC void lc_rt_release(uintptr_t p)
{
    int i;

    for (i = 0; i < lc_rt_nwatched; i++)
        if (lc_rt_watched[i] == p)
            lc_rt_given_back[i] = 1;
}

/* The address is kept as a number: once realloc has moved a block, the
 * old pointer may not even be looked at. A realloc that fails leaves the
 * array as it was. */
LC_RT_LIBC void *lc_rt_realloc(void *p, size_t size)
{
    uintptr_t old = (uintptr_t)p;
    void *q = realloc(p, size);

    if (old && (q || size == 0))
        lc_rt_release(old);
    return q;
}

LC_RT_LIBC void lc_rt_free(void *p)
{
    if (p)
        lc_rt_release((uintptr_t)p);
    free(p);
}

#define qsort lc_rt_qsort
#define realloc lc_rt_realloc
#define free lc_rt_free
