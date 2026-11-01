/* Reference solution: count each digit's occurrences in one pass, then
 * check that the digit at every position i equals the count of digit i. */
bool digitCount(char *num)
{
    int count[10] = {0}, n = 0, i;

    for (; num[n] != '\0'; n++)
        count[num[n] - '0']++;
    for (i = 0; i < n; i++)
        if (count[i] != num[i] - '0')
            return false;
    return true;
}
