/* Crashes on odd input. */
int half(int x) {
    if (x % 2) {
        volatile int *volatile p = NULL;
        *p = 1;
    }
    return x / 2;
}
