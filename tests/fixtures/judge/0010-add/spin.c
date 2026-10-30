/* Never returns on the third case. */
static volatile unsigned long n;
int add(int a, int b) {
    while (a == 100)
        n++;
    return a + b;
}
