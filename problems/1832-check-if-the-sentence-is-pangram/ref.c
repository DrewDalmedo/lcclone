/* Reference solution: set one bit of a 26-bit mask for each letter seen,
 * and compare the mask with the one that has all 26 bits set. */
bool checkIfPangram(char *sentence)
{
    unsigned long seen = 0;
    const unsigned long all = (1UL << 26) - 1;
    const char *p;

    for (p = sentence; *p; p++)
        seen |= 1UL << (*p - 'a');
    return seen == all;
}
