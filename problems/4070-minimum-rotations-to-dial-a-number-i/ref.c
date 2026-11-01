/* Reference solution: each digit is reached from where the knob stopped
 * for the previous one (from 0 for the first), and the cheaper way round
 * between digits a and b takes min(|a - b|, 10 - |a - b|) rotations. The
 * steps don't affect each other, so their minima add up. */
int minRotations(char *s)
{
    int at = 0, total = 0, d, i;

    for (i = 0; s[i]; i++) {
        d = abs(s[i] - '0' - at);
        total += d < 10 - d ? d : 10 - d;
        at = s[i] - '0';
    }
    return total;
}
