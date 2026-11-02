/*
 * lc_rt.c: the C harness runtime. It follows lc_rt_core.h in every C
 * build and gives the generated harness a reader and a writer for each
 * type, in LeetCode's C conventions: an array comes with its size, a 2D
 * array with its row count and an array of row sizes, and everything is
 * malloc'ed, so a solution may modify, keep, realloc or free what it is
 * given. lc_rt_libc.h, before the solution, notes which array parameters
 * it reallocated or freed.
 *
 * Writers take the solution's function name, for the message when what
 * the solution returned can't be written, such as a NULL array. For a
 * problem solved in place (check:), the harness sets lc_rt_param to the
 * parameter being written, and the messages speak of it instead.
 */

static const char *lc_rt_param;

/* ------------------------------------------------------------------ */
/* Scalars                                                             */
/* ------------------------------------------------------------------ */

LC_RT int lc_read_int(void)
{
    return (int)lc_rt_ll(INT_MIN, INT_MAX);
}

LC_RT long long lc_read_long(void)
{
    return lc_rt_ll(LLONG_MIN, LLONG_MAX);
}

LC_RT uint32_t lc_read_uint32(void)
{
    return (uint32_t)lc_rt_ll(0, 4294967295LL);
}

LC_RT double lc_read_double(void)
{
    return lc_rt_double();
}

LC_RT bool lc_read_bool(void)
{
    return lc_rt_bool() != 0;
}

LC_RT char lc_read_char(void)
{
    return (char)(unsigned char)lc_rt_ll(0, 255);
}

LC_RT char *lc_read_string(void)
{
    size_t len;

    return lc_rt_string(&len);
}

LC_RT void lc_write_int(const char *fn, int v)
{
    (void)fn;
    lc_rt_put_ll(v);
}

LC_RT void lc_write_long(const char *fn, long long v)
{
    (void)fn;
    lc_rt_put_ll(v);
}

LC_RT void lc_write_uint32(const char *fn, uint32_t v)
{
    (void)fn;
    lc_rt_put_ull(v);
}

LC_RT void lc_write_double(const char *fn, double v)
{
    (void)fn;
    lc_rt_put_double(v);
}

LC_RT void lc_write_bool(const char *fn, bool v)
{
    (void)fn;
    lc_rt_put_ll(v ? 1 : 0);
}

LC_RT void lc_write_char(const char *fn, char v)
{
    (void)fn;
    lc_rt_put_ll((unsigned char)v);
}

LC_RT void lc_write_string(const char *fn, const char *s)
{
    if (!s && lc_rt_param)
        lc_rt_fail("%s left a NULL string in %s", fn, lc_rt_param);
    if (!s)
        lc_rt_fail("%s returned a NULL string", fn);
    lc_rt_put_string(s, strlen(s));
}

/* ------------------------------------------------------------------ */
/* Arrays                                                              */
/* ------------------------------------------------------------------ */

/* The size a function reported, which lc set to -1 before the call. */
LC_RT void lc_rt_check_size(const char *fn, int n)
{
    if (n == -1)
        lc_rt_fail("%s did not set *returnSize", fn);
    if (n < 0)
        lc_rt_fail("%s set *returnSize to %d", fn, n);
}

/*
 * Whether this build has AddressSanitizer: gcc says so with
 * __SANITIZE_ADDRESS__, clang with __has_feature.
 */
#if defined(__SANITIZE_ADDRESS__)
#define LC_RT_ASAN 1
#elif defined(__has_feature)
#if __has_feature(address_sanitizer)
#define LC_RT_ASAN 1
#endif
#endif

/*
 * The spare zeroed elements an array gets after the ones it holds. A
 * solution that changes the spare one wrote past the end of the array,
 * which LeetCode reports as a heap buffer overflow, and lc_check_N_end
 * says so. Under AddressSanitizer there is no spare element: ASan itself
 * then reports a write past the end, and a read too, as LeetCode's does.
 * A char array always has one, so that it is also a NUL-terminated
 * string.
 */
#ifdef LC_RT_ASAN
#define LC_RT_SPARE 0
#else
#define LC_RT_SPARE 1
#endif

/*
 * For each type N with C type T, LC_RT_ARRAYS defines
 *   T *lc_read_N_array(int *size)
 *   T **lc_read_N_array2(int *size, int **colSizes)
 *   void lc_write_N_array(fn, T *a, int size)
 *   void lc_write_N_array2(fn, T **a, int size, int *colSizes)
 *   void lc_check_N_end(fn, param, T *a, int size)
 * An array of N gets SPARE spare elements; the row and column-size arrays
 * of a 2D array get LC_RT_SPARE. lc_check_N_end, called after the
 * solution returns, fails when the spare element is no longer all zero
 * bits, and does nothing when there is none.
 *
 * An array parameter (lc_read_N_array, not the rows of a 2D one) is
 * watched: once the solution has freed or reallocated it, its memory is
 * no longer the harness's to read. lc_check_N_end then leaves it alone,
 * and lc_write_N_array fails if it is the parameter that holds the result
 * (check:), whose memory LeetCode's harness would read after it was
 * freed: AddressSanitizer reports that as a heap-use-after-free there.
 */
#define LC_RT_ARRAYS(N, T, SPARE)                                          \
    LC_RT T *lc_rt_read_##N##_items(int *size)                             \
    {                                                                      \
        int n = lc_rt_count(), i;                                          \
        T *a = (T *)lc_rt_alloc((size_t)n + (SPARE), sizeof *a);           \
        for (i = 0; i < n; i++)                                            \
            a[i] = lc_read_##N();                                          \
        *size = n;                                                         \
        return a;                                                          \
    }                                                                      \
    LC_RT T *lc_read_##N##_array(int *size)                                \
    {                                                                      \
        T *a = lc_rt_read_##N##_items(size);                               \
        lc_rt_watch(a);                                                    \
        return a;                                                          \
    }                                                                      \
    LC_RT T **lc_read_##N##_array2(int *size, int **colSizes)              \
    {                                                                      \
        int n = lc_rt_count(), i;                                          \
        T **a = (T **)lc_rt_alloc((size_t)n + LC_RT_SPARE, sizeof *a);     \
        int *cols =                                                        \
            (int *)lc_rt_alloc((size_t)n + LC_RT_SPARE, sizeof *cols);     \
        for (i = 0; i < n; i++)                                            \
            a[i] = lc_rt_read_##N##_items(&cols[i]);                       \
        *size = n;                                                         \
        *colSizes = cols;                                                  \
        return a;                                                          \
    }                                                                      \
    LC_RT void lc_write_##N##_array(const char *fn, T *a, int n)           \
    {                                                                      \
        int i;                                                             \
        if (lc_rt_param && lc_rt_released(a))                              \
            lc_rt_fail("%s freed or reallocated %s, which holds the "      \
                       "result", fn, lc_rt_param);                         \
        lc_rt_check_size(fn, n);                                           \
        if (!a && n > 0)                                                   \
            lc_rt_fail("%s returned NULL, but *returnSize is %d", fn, n);  \
        lc_rt_put_ll(n);                                                   \
        for (i = 0; i < n; i++)                                            \
            lc_write_##N(fn, a[i]);                                        \
    }                                                                      \
    LC_RT void lc_write_##N##_array2(const char *fn, T **a, int n,         \
                                     int *colSizes)                        \
    {                                                                      \
        int i, j;                                                          \
        lc_rt_check_size(fn, n);                                           \
        if (!a && n > 0)                                                   \
            lc_rt_fail("%s returned NULL, but *returnSize is %d", fn, n);  \
        if (!colSizes && n > 0)                                            \
            lc_rt_fail("%s did not set *returnColumnSizes", fn);           \
        lc_rt_put_ll(n);                                                   \
        for (i = 0; i < n; i++) {                                          \
            if (colSizes[i] < 0 && lc_rt_param)                            \
                lc_rt_fail("%s set %sColSize[%d] to %d", fn, lc_rt_param,  \
                           i, colSizes[i]);                                \
            if (colSizes[i] < 0)                                           \
                lc_rt_fail("%s set (*returnColumnSizes)[%d] to %d", fn, i, \
                           colSizes[i]);                                   \
            if (!a[i] && colSizes[i] > 0 && lc_rt_param)                   \
                lc_rt_fail("%s left %s[%d] NULL, but its size is %d", fn,  \
                           lc_rt_param, i, colSizes[i]);                   \
            if (!a[i] && colSizes[i] > 0)                                  \
                lc_rt_fail("%s returned NULL as row %d, whose size is %d", \
                           fn, i, colSizes[i]);                            \
            lc_rt_put_ll(colSizes[i]);                                     \
            for (j = 0; j < colSizes[i]; j++)                              \
                lc_write_##N(fn, a[i][j]);                                 \
        }                                                                  \
    }                                                                      \
    LC_RT void lc_check_##N##_end(const char *fn, const char *param, T *a, \
                                  int n)                                   \
    {                                                                      \
        T zero;                                                            \
        memset(&zero, 0, sizeof zero);                                     \
        if ((SPARE) > 0 && !lc_rt_released(a) &&                           \
            memcmp(&a[n], &zero, sizeof zero) != 0)                        \
            lc_rt_fail("%s wrote past the end of %s (%s[%d])", fn, param,  \
                       param, n);                                          \
    }

LC_RT_ARRAYS(int, int, LC_RT_SPARE)
LC_RT_ARRAYS(long, long long, LC_RT_SPARE)
LC_RT_ARRAYS(uint32, uint32_t, LC_RT_SPARE)
LC_RT_ARRAYS(double, double, LC_RT_SPARE)
LC_RT_ARRAYS(bool, bool, LC_RT_SPARE)
LC_RT_ARRAYS(char, char, 1)
LC_RT_ARRAYS(string, char *, LC_RT_SPARE)
