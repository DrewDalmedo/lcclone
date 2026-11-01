/* Reference solution: a move only ever erases a second copy of a letter,
 * and while a letter has two copies one of them can erase the other, so
 * exactly one copy of each letter survives. Count the different letters
 * with a 26-bit mask. */
int minimizedStringLength(char *s)
{
    unsigned long seen = 0;
    int count = 0;
    const char *p;

    for (p = s; *p; p++) {
        unsigned long bit = 1UL << (*p - 'a');

        if (!(seen & bit)) {
            seen |= bit;
            count++;
        }
    }
    return count;
}
