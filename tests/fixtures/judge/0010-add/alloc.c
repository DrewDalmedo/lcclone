/* Uses about 300 MB on the third case; the limit is 64 MB. */
int add(int a, int b) {
    if (a == 100) {
        for (int i = 0; i < 300; i++) {
            volatile char *p = malloc(1 << 20);
            if (!p)
                abort();
            for (int k = 0; k < (1 << 20); k += 4096)
                p[k] = 1;
        }
    }
    return a + b;
}
