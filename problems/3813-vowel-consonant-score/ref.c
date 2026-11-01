/* Reference solution: one pass counting vowels and the other letters
 * (digits and spaces are skipped), then integer division, which rounds
 * down for non-negative numbers; 0 when there is no consonant. */
int vowelConsonantScore(char *s)
{
    int v = 0, c = 0;

    for (; *s; s++) {
        if (*s < 'a' || *s > 'z')
            continue;
        switch (*s) {
        case 'a': case 'e': case 'i': case 'o': case 'u':
            v++;
            break;
        default:
            c++;
        }
    }
    return c > 0 ? v / c : 0;
}
