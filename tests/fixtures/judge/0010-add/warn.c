/* Compiles with a warning that gcc and clang give by default. */
static int *local(void) {
    int x = 0;
    return &x;
}

int add(int a, int b) {
    (void)local;
    return a + b;
}
