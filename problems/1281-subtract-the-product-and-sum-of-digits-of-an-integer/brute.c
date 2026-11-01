/* Slow and obviously right: print n as text and go through its digit
 * characters one by one. */
int subtractProductAndSum(int n)
{
    char text[16];
    int product = 1, sum = 0, i;

    snprintf(text, sizeof text, "%d", n);
    for (i = 0; text[i] != '\0'; i++) {
        product *= text[i] - '0';
        sum += text[i] - '0';
    }
    return product - sum;
}
