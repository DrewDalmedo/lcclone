/* Prints 2 MB on the third case; the limit is 1 MiB. */
int add(int a, int b) {
    if (a == 100)
        for (int i = 0; i < 2 * 1024 * 1024 / 16; i++)
            fputs("0123456789abcde\n", stdout);
    return a + b;
}
