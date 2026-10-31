/* Crashes on the third case (a = 100). */
int add(int a, int b) {
    if (a == 100) {
        volatile int *volatile p = NULL;
        *p = 1;
    }
    return a + b;
}
