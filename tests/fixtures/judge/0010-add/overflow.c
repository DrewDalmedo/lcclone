/* Reads past the end of an array: only the sanitizers notice. */
int add(int a, int b) {
    int *v = malloc(2 * sizeof *v);
    v[0] = a;
    v[1] = b;
    if (a == 100) {
        volatile int past = v[2];
        (void)past;
    }
    int r = v[0] + v[1];
    free(v);
    return r;
}
