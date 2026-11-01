/* Reference solution: mark the kinds of jewels in a table indexed by the
 * character itself, then count the stones whose character is marked. */
int numJewelsInStones(char *jewels, char *stones)
{
    char keep[128] = {0};
    int count = 0;

    for (; *jewels; jewels++)
        keep[(unsigned char)*jewels] = 1;
    for (; *stones; stones++)
        count += keep[(unsigned char)*stones];
    return count;
}
