/* Helpers named like BSD functions that macOS's <stdlib.h>, <string.h>,
 * <strings.h> and <ctype.h> declare, with types of their own, and enum
 * constants named like the SVID macros of its <math.h>. LeetCode's C has
 * none of these names, so this builds there. */
enum { UNDERFLOW = -1, OVERFLOW = 1 };

void mergesort(int *a, int n) {
    for (int i = 1; i < n; i++)
        for (int j = i; j > 0 && a[j - 1] > a[j]; j--) {
            int t = a[j];
            a[j] = a[j - 1];
            a[j - 1] = t;
        }
}

int heapsort(int *a, int n) {
    mergesort(a, n);
    return a[0];
}

long radixsort(long x) { return x; }
int fls(int x) { return x; }
char *strnstr(char *s) { return s; }
bool isnumber(char c) { return c >= '0' && c <= '9'; }

int add(int a, int b) {
    int v[2] = {b, a};
    char s[] = "7";
    int lo = heapsort(v, 2);

    return lo + v[1] + (int)radixsort(0) + fls(0) +
           (isnumber(*strnstr(s)) ? OVERFLOW + UNDERFLOW : 1);
}
