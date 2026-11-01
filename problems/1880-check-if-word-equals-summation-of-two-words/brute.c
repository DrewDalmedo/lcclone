/* Slow and obviously right, with no integer values at all: add the first
 * two words column by column from the right, as on paper, then compare the
 * digits of the sum with the target's, skipping leading zeros of both. */
bool isSumEqual(char *firstWord, char *secondWord, char *targetWord)
{
    char sum[10], *p, *q;
    int i = (int)strlen(firstWord) - 1, j = (int)strlen(secondWord) - 1;
    int k = 9, carry = 0, d;

    sum[k] = '\0';
    while (i >= 0 || j >= 0 || carry > 0) {
        d = carry;
        if (i >= 0)
            d += firstWord[i--] - 'a';
        if (j >= 0)
            d += secondWord[j--] - 'a';
        sum[--k] = (char)('0' + d % 10);
        carry = d / 10;
    }
    for (p = sum + k; *p == '0'; p++)
        ;
    for (q = targetWord; *q == 'a'; q++)
        ;
    for (; *p != '\0' && *q != '\0'; p++, q++)
        if (*p - '0' != *q - 'a')
            return false;
    return *p == '\0' && *q == '\0';
}
