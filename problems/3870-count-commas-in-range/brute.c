/* Slow and obviously right: write every number out with its commas, as
 * the statement describes, and count the commas in the text. */
int countCommas(int n)
{
    char digits[16], text[32];
    int x, len, i, k, total = 0;

    for (x = 1; x <= n; x++) {
        len = sprintf(digits, "%d", x);
        k = 0;
        for (i = 0; i < len; i++) {
            if (i > 0 && (len - i) % 3 == 0)
                text[k++] = ',';
            text[k++] = digits[i];
        }
        for (i = 0; i < k; i++)
            if (text[i] == ',')
                total++;
    }
    return total;
}
