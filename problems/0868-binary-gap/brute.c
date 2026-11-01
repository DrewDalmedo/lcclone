/* Slow and obviously right: write n out as binary digits, then check every
 * pair of 1s and keep the widest pair with no other 1 between them. */
int binaryGap(int n)
{
    char bits[32];
    int len = 0, i, j, k, best = 0, clear;

    while (n > 0) {
        bits[len++] = (char)('0' + n % 2);
        n /= 2;
    }
    for (i = 0; i < len; i++)
        for (j = i + 1; j < len; j++) {
            if (bits[i] != '1' || bits[j] != '1')
                continue;
            clear = 1;
            for (k = i + 1; k < j; k++)
                if (bits[k] == '1')
                    clear = 0;
            if (clear && j - i > best)
                best = j - i;
        }
    return best;
}
