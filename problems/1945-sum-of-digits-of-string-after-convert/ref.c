/* Reference solution: the first fold of the spelled-out number adds up
 * the digits of each letter's rank, so it is done letter by letter and the
 * long number is never built. The remaining k - 1 folds act on an int. */
int getLucky(char *s, int k)
{
    int value = 0, rank, next, i;

    for (i = 0; s[i] != '\0'; i++) {
        rank = s[i] - 'a' + 1;
        value += rank / 10 + rank % 10;
    }
    for (i = 1; i < k; i++) {
        for (next = 0; value > 0; value /= 10)
            next += value % 10;
        value = next;
    }
    return value;
}
