/* Reference solution: one pass with a single balance, adding 1 for each
 * vowel of the left half and taking 1 away for each vowel of the right
 * half; the halves match when the balance ends at 0. */
static int is_vowel(char c)
{
    switch (c) {
    case 'a': case 'e': case 'i': case 'o': case 'u':
    case 'A': case 'E': case 'I': case 'O': case 'U':
        return 1;
    default:
        return 0;
    }
}

bool halvesAreAlike(char *s)
{
    int n = (int)strlen(s), half = n / 2, balance = 0, i;

    for (i = 0; i < n; i++)
        if (is_vowel(s[i]))
            balance += i < half ? 1 : -1;
    return balance == 0;
}
