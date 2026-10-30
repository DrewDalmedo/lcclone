/* Wrong when a is negative. */
int add(int a, int b) {
    return a < 0 ? a - b : a + b;
}
