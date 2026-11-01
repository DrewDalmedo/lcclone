/* Reference solution: one pass sets bit i of `low` or `cap` for
 * letter i in that form; then look from Z down for the first letter whose
 * bit is set in both. The answer goes in a malloc'ed string of length 1,
 * or 0 when there is none. */
char *greatestLetter(char *s)
{
    unsigned low = 0, cap = 0, both;
    char *out = malloc(2);
    int i;

    for (; *s; s++) {
        if (*s >= 'a' && *s <= 'z')
            low |= 1u << (*s - 'a');
        else
            cap |= 1u << (*s - 'A');
    }
    both = low & cap;
    out[0] = '\0';
    out[1] = '\0';
    for (i = 25; i >= 0; i--)
        if (both & (1u << i)) {
            out[0] = (char)('A' + i);
            break;
        }
    return out;
}
