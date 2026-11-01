/* Reference solution: scan s, keeping the letters read so far as bits of a
 * mask, and stop at the first letter whose bit is already set. */
char repeatedCharacter(char *s)
{
    int seen = 0, bit;

    for (; *s; s++) {
        bit = 1 << (*s - 'a');
        if (seen & bit)
            return *s;
        seen |= bit;
    }
    return '\0';
}
