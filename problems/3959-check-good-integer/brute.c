/* Slow and obviously right: write n out as text, then work out both sums
 * from its characters, separately, and compare them at the end. */
bool checkGoodInteger(int n)
{
    char text[16];
    int i, d, sum = 0, squares = 0;

    snprintf(text, sizeof text, "%d", n);
    for (i = 0; text[i] != '\0'; i++) {
        d = text[i] - '0';
        sum += d;
        squares += d * d;
    }
    return squares - sum >= 50;
}
