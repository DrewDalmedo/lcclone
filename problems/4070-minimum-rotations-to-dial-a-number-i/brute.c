/* Slow and obviously right: for each digit, turn the knob one position at
 * a time in one direction until the arrow shows the digit, count the
 * rotations, do the same in the other direction, and keep the smaller
 * count. */
int minRotations(char *s)
{
    int at = 0, total = 0, want, up, down, pos, i;

    for (i = 0; s[i]; i++) {
        want = s[i] - '0';
        up = 0;
        for (pos = at; pos != want; pos = pos == 9 ? 0 : pos + 1)
            up++;
        down = 0;
        for (pos = at; pos != want; pos = pos == 0 ? 9 : pos - 1)
            down++;
        total += up < down ? up : down;
        at = want;
    }
    return total;
}
