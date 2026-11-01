/* Reference solution: a prefix can be laid out with neighbours always
 * different exactly when its numbers of 0s and 1s differ by at most one
 * (the more common character takes every other place, starting with the
 * first). Keep the running difference and count the prefixes where it is
 * -1, 0 or 1. */
int countValidPrefixes(char *s)
{
    int diff = 0, count = 0, i;

    for (i = 0; s[i] != '\0'; i++) {
        diff += s[i] == '0' ? 1 : -1;
        if (diff >= -1 && diff <= 1)
            count++;
    }
    return count;
}
