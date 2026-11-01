/* Reference solution: one pass sets a bit per letter in a mask of the
 * lowercase forms seen and in a mask of the uppercase forms seen; the
 * answer is the number of bits the two masks share. */
int numberOfSpecialChars(char *word)
{
    unsigned lower = 0, upper = 0, both;
    int count = 0;
    const char *p;

    for (p = word; *p; p++) {
        if (*p >= 'a' && *p <= 'z')
            lower |= 1u << (*p - 'a');
        else
            upper |= 1u << (*p - 'A');
    }
    for (both = lower & upper; both; both &= both - 1)
        count++;
    return count;
}
