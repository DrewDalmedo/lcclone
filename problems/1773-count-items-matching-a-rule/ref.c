/* Reference solution: turn ruleKey into a column number once, then count
 * the rows whose word in that column equals ruleValue as a whole. */
int countMatches(char ***items, int itemsSize, int *itemsColSize,
                 char *ruleKey, char *ruleValue)
{
    int col = strcmp(ruleKey, "type") == 0    ? 0
              : strcmp(ruleKey, "color") == 0 ? 1
                                              : 2;
    int count = 0, i;

    (void)itemsColSize;
    for (i = 0; i < itemsSize; i++)
        if (strcmp(items[i][col], ruleValue) == 0)
            count++;
    return count;
}
