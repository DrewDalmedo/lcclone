/* Slow and obviously right: write num out, turn each digit in turn (6 to
 * 9 or 9 to 6), and keep the largest number seen, num itself included. */
int maximum69Number(int num)
{
    char buf[16], c;
    int best = num, len, i, v;

    len = snprintf(buf, sizeof buf, "%d", num);
    for (i = 0; i < len; i++) {
        c = buf[i];
        buf[i] = c == '6' ? '9' : '6';
        v = atoi(buf);
        if (v > best)
            best = v;
        buf[i] = c;
    }
    return best;
}
