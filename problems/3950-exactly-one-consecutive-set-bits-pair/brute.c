/* Slow and obviously right: write n out in binary as a string of '0' and
 * '1' characters, then look at every two neighbouring characters, from
 * each starting position, and count those that are both '1'. */
bool consecutiveSetBits(int n)
{
    char bits[40];
    int len = 0, i, pairs = 0;

    do {
        bits[len++] = (char)('0' + n % 2);
        n /= 2;
    } while (n > 0);
    for (i = 0; i + 1 < len; i++)
        if (bits[i] == '1' && bits[i + 1] == '1')
            pairs++;
    return pairs == 1;
}
