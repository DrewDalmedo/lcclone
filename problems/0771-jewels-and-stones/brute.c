/* Slow and obviously right: for each stone, compare it with every
 * character of jewels, and count it when one of them is equal. */
int numJewelsInStones(char *jewels, char *stones)
{
    size_t i, k;
    int count = 0, found;

    for (i = 0; stones[i] != '\0'; i++) {
        found = 0;
        for (k = 0; jewels[k] != '\0'; k++)
            if (stones[i] == jewels[k])
                found = 1;
        count += found;
    }
    return count;
}
