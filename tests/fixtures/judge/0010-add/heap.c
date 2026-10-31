/* Writes past the end of a heap array when a is 100: AddressSanitizer
 * reports it with a stack trace, whose frames should name this file. Not
 * inlined into the harness's main(), so that a frame of its own names it
 * whatever the compiler decides. */
__attribute__((noinline)) int add(int a, int b) {
    int n = a == 100 ? 2 : 3;
    volatile int *v = malloc((size_t)n * sizeof *v);
    v[2] = a;
    free((void *)v);
    return a + b;
}
