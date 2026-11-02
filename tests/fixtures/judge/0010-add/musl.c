/* Helpers named like functions that musl's <stdlib.h> and <stdio.h>
 * declare with _DEFAULT_SOURCE, and glibc's only with _GNU_SOURCE or
 * _XOPEN_SOURCE, with types of their own, and a variable named like
 * <math.h>'s HUGE. LeetCode's C has none of these names, so this builds
 * there. */
double HUGE = 1e18;

/* A sort with a context argument, like GNU's qsort_r. */
void qsort_r(int *a, int n, const int *sign) {
    for (int i = 1; i < n; i++)
        for (int j = i; j > 0 && *sign * a[j - 1] > *sign * a[j]; j--) {
            int t = a[j];
            a[j] = a[j - 1];
            a[j - 1] = t;
        }
}

int mkostemp(int a) { return a; }
int mkostemps(int a, int b) { return a - b; }
long memalign(long x) { return x; }
int setkey(int k) { return k; }
int grantpt(void) { return 0; }
int unlockpt(void) { return 0; }
int posix_openpt(void) { return 0; }
char *ptsname(char *s) { return s; }
const char *cuserid(void) { return "lc"; }
long fgetln(long x) { return x; }

int add(int a, int b) {
    int v[2] = {b, a};
    int up = 1;
    char s[] = "x";

    qsort_r(v, 2, &up);
    if (v[0] + (double)v[1] > HUGE)
        return 0;
    return v[0] + v[1] + mkostemps(mkostemp(1), 1) + (int)memalign(0) +
           setkey(0) + grantpt() + unlockpt() + posix_openpt() +
           (int)fgetln(0) + (*ptsname(s) == 'x' && *cuserid() == 'l' ? 0 : 1);
}
